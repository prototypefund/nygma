// SPDX-License-Identifier: Apache-2.0

// @authors
//   mb <mb@64k.by>
//   - <https://github.com/lemire/streamvbyte/blob/master/AUTHORS>

#pragma once

// this implementation differs from the original streamvbyte in that
// the control bytes get stored interleaved with the data bytes

#include <libriot/compression/delta-simd.hxx>
#include <libriot/compression/integer.hxx>

#include <cstddef>
#include <cstdint>

#include <algorithm>

#include <immintrin.h>

namespace riot::streamvbyte {

namespace detail {

namespace {

constexpr uint8_t shuffle_lut[64 * 16]{
    0x00, 0x04, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x04, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x0e, 0x0f, 0xff,
    0x00, 0x04, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff,
    0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

// clang-format off
constexpr std::uint8_t decode_shuffle_lut[256][16] = {
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 0xff, 0xff, 0xff },   // 0000
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff },   // 1000
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff },   // 2000
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 3000
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff },   // 0100
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff },   // 1100
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 2100
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 3100
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff },   // 0200
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 0xff, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 1200
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 2200
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff, 8, 0xff, 0xff, 0xff },   // 3200
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 0xff, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 0300
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 0xff, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 1300
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff, 8, 0xff, 0xff, 0xff },   // 2300
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff, 9, 0xff, 0xff, 0xff },   // 3300
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 0xff, 0xff, 0xff },   // 0010
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff },   // 1010
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 2010
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 3010
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff },   // 0110
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 1110
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 2110
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff, 8, 0xff, 0xff, 0xff },   // 3110
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff },   // 0210
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 1210
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff, 8, 0xff, 0xff, 0xff },   // 2210
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff, 9, 0xff, 0xff, 0xff },   // 3210
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 0xff, 0xff, 7, 0xff, 0xff, 0xff },   // 0310
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 0xff, 0xff, 8, 0xff, 0xff, 0xff },   // 1310
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 9, 0xff, 0xff, 0xff },   // 2310
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 10, 0xff, 0xff, 0xff },   // 3310
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 0xff, 5, 0xff, 0xff, 0xff },   // 0020
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff },   // 1020
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff },   // 2020
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff, 8, 0xff, 0xff, 0xff },   // 3020
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff },   // 0120
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff },   // 1120
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff, 8, 0xff, 0xff, 0xff },   // 2120
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff, 9, 0xff, 0xff, 0xff },   // 3120
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff },   // 0220
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 0xff, 8, 0xff, 0xff, 0xff },   // 1220
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff, 9, 0xff, 0xff, 0xff },   // 2220
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 0xff, 10, 0xff, 0xff, 0xff },   // 3220
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 0xff, 8, 0xff, 0xff, 0xff },   // 0320
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 0xff, 9, 0xff, 0xff, 0xff },   // 1320
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff, 10, 0xff, 0xff, 0xff },   // 2320
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 11, 0xff, 0xff, 0xff },   // 3320
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 5, 6, 0xff, 0xff, 0xff },   // 0030
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff },   // 1030
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff },   // 2030
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff },   // 3030
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff },   // 0130
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff },   // 1130
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff },   // 2130
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 9, 10, 0xff, 0xff, 0xff },   // 3130
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff },   // 0230
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff },   // 1230
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9, 10, 0xff, 0xff, 0xff },   // 2230
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 10, 11, 0xff, 0xff, 0xff },   // 3230
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 0xff },   // 0330
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 0xff, 0xff },   // 1330
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0xff, 0xff, 0xff },   // 2330
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0xff, 0xff, 0xff },   // 3330
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 0xff, 0xff },   // 0001
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff },   // 1001
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff },   // 2001
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 3001
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff },   // 0101
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff },   // 1101
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 2101
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 3101
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff },   // 0201
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 1201
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 2201
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff, 8, 9, 0xff, 0xff },   // 3201
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 0xff, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 0301
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 0xff, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 1301
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff, 8, 9, 0xff, 0xff },   // 2301
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff, 9, 10, 0xff, 0xff },   // 3301
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 0xff, 0xff },   // 0011
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff },   // 1011
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 2011
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 3011
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff },   // 0111
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 1111
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 2111
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff, 8, 9, 0xff, 0xff },   // 3111
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff },   // 0211
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 1211
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff, 8, 9, 0xff, 0xff },   // 2211
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff, 9, 10, 0xff, 0xff },   // 3211
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 0xff, 0xff, 7, 8, 0xff, 0xff },   // 0311
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 0xff, 0xff, 8, 9, 0xff, 0xff },   // 1311
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 9, 10, 0xff, 0xff },   // 2311
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 10, 11, 0xff, 0xff },   // 3311
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 0xff, 0xff },   // 0021
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff },   // 1021
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff },   // 2021
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 0xff, 0xff },   // 3021
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff },   // 0121
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff },   // 1121
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 0xff, 0xff },   // 2121
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff, 9, 10, 0xff, 0xff },   // 3121
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff },   // 0221
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 0xff, 8, 9, 0xff, 0xff },   // 1221
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff, 9, 10, 0xff, 0xff },   // 2221
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 0xff, 10, 11, 0xff, 0xff },   // 3221
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 0xff, 8, 9, 0xff, 0xff },   // 0321
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 0xff, 9, 10, 0xff, 0xff },   // 1321
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff, 10, 11, 0xff, 0xff },   // 2321
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 11, 12, 0xff, 0xff },   // 3321
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 0xff, 0xff },   // 0031
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff },   // 1031
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 0xff, 0xff },   // 2031
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 0xff, 0xff },   // 3031
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff },   // 0131
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 0xff, 0xff },   // 1131
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 0xff, 0xff },   // 2131
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 9, 10, 11, 0xff, 0xff },   // 3131
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 7, 8, 9, 0xff, 0xff },   // 0231
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 8, 9, 10, 0xff, 0xff },   // 1231
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9, 10, 11, 0xff, 0xff },   // 2231
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 10, 11, 12, 0xff, 0xff },   // 3231
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 0xff },   // 0331
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0xff, 0xff },   // 1331
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0xff, 0xff },   // 2331
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0xff, 0xff },   // 3331
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 0xff },   // 0002
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff },   // 1002
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff },   // 2002
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 0xff },   // 3002
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff },   // 0102
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff },   // 1102
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 0xff },   // 2102
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 9, 0xff },   // 3102
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff },   // 0202
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 0xff },   // 1202
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 9, 0xff },   // 2202
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff, 8, 9, 10, 0xff },   // 3202
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 0xff, 0xff, 0xff, 6, 7, 8, 0xff },   // 0302
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 0xff, 0xff, 0xff, 7, 8, 9, 0xff },   // 1302
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff, 8, 9, 10, 0xff },   // 2302
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff, 9, 10, 11, 0xff },   // 3302
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 0xff },   // 0012
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff },   // 1012
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff },   // 2012
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 0xff },   // 3012
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff },   // 0112
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff },   // 1112
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 0xff },   // 2112
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff, 8, 9, 10, 0xff },   // 3112
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff },   // 0212
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 0xff },   // 1212
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff, 8, 9, 10, 0xff },   // 2212
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff, 9, 10, 11, 0xff },   // 3212
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 0xff, 0xff, 7, 8, 9, 0xff },   // 0312
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 0xff, 0xff, 8, 9, 10, 0xff },   // 1312
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 9, 10, 11, 0xff },   // 2312
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 10, 11, 12, 0xff },   // 3312
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 0xff },   // 0022
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff },   // 1022
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 0xff },   // 2022
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 0xff },   // 3022
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff },   // 0122
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 0xff },   // 1122
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 0xff },   // 2122
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff, 9, 10, 11, 0xff },   // 3122
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 0xff },   // 0222
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 0xff },   // 1222
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff, 9, 10, 11, 0xff },   // 2222
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 0xff, 10, 11, 12, 0xff },   // 3222
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 0xff, 8, 9, 10, 0xff },   // 0322
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 0xff, 9, 10, 11, 0xff },   // 1322
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff, 10, 11, 12, 0xff },   // 2322
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 11, 12, 13, 0xff },   // 3322
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 0xff },   // 0032
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff },   // 1032
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 10, 0xff },   // 2032
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 11, 0xff },   // 3032
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff },   // 0132
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 10, 0xff },   // 1132
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 11, 0xff },   // 2132
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 9, 10, 11, 12, 0xff },   // 3132
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 7, 8, 9, 10, 0xff },   // 0232
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 8, 9, 10, 11, 0xff },   // 1232
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9, 10, 11, 12, 0xff },   // 2232
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 10, 11, 12, 13, 0xff },   // 3232
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0xff },   // 0332
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0xff },   // 1332
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0xff },   // 2332
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0xff },   // 3332
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 6 },   // 0003
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7 },   // 1003
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8 },   // 2003
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 9 },   // 3003
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7 },   // 0103
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8 },   // 1103
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 9 },   // 2103
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 9, 10 },   // 3103
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8 },   // 0203
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 0xff, 0xff, 0xff, 6, 7, 8, 9 },   // 1203
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 0xff, 0xff, 0xff, 7, 8, 9, 10 },   // 2203
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 0xff, 0xff, 0xff, 8, 9, 10, 11 },   // 3203
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 0xff, 0xff, 0xff, 6, 7, 8, 9 },   // 0303
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 0xff, 0xff, 0xff, 7, 8, 9, 10 },   // 1303
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 0xff, 0xff, 0xff, 8, 9, 10, 11 },   // 2303
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 0xff, 9, 10, 11, 12 },   // 3303
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 7 },   // 0013
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8 },   // 1013
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 9 },   // 2013
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 10 },   // 3013
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8 },   // 0113
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 9 },   // 1113
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 10 },   // 2113
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 0xff, 0xff, 8, 9, 10, 11 },   // 3113
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 0xff, 0xff, 6, 7, 8, 9 },   // 0213
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 0xff, 0xff, 7, 8, 9, 10 },   // 1213
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 0xff, 0xff, 8, 9, 10, 11 },   // 2213
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 0xff, 0xff, 9, 10, 11, 12 },   // 3213
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 0xff, 0xff, 7, 8, 9, 10 },   // 0313
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 0xff, 0xff, 8, 9, 10, 11 },   // 1313
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 0xff, 0xff, 9, 10, 11, 12 },   // 2313
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xff, 10, 11, 12, 13 },   // 3313
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 8 },   // 0023
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9 },   // 1023
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 10 },   // 2023
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 11 },   // 3023
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9 },   // 0123
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 10 },   // 1123
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 11 },   // 2123
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 0xff, 9, 10, 11, 12 },   // 3123
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 0xff, 7, 8, 9, 10 },   // 0223
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 0xff, 8, 9, 10, 11 },   // 1223
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 0xff, 9, 10, 11, 12 },   // 2223
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 0xff, 10, 11, 12, 13 },   // 3223
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 0xff, 8, 9, 10, 11 },   // 0323
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 0xff, 9, 10, 11, 12 },   // 1323
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 0xff, 10, 11, 12, 13 },   // 2323
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xff, 11, 12, 13, 14 },   // 3323
    { 0, 0xff, 0xff, 0xff, 1, 0xff, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9 },   // 0033
    { 0, 1, 0xff, 0xff, 2, 0xff, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 9, 10 },   // 1033
    { 0, 1, 2, 0xff, 3, 0xff, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 10, 11 },   // 2033
    { 0, 1, 2, 3, 4, 0xff, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 11, 12 },   // 3033
    { 0, 0xff, 0xff, 0xff, 1, 2, 0xff, 0xff, 3, 4, 5, 6, 7, 8, 9, 10 },   // 0133
    { 0, 1, 0xff, 0xff, 2, 3, 0xff, 0xff, 4, 5, 6, 7, 8, 9, 10, 11 },   // 1133
    { 0, 1, 2, 0xff, 3, 4, 0xff, 0xff, 5, 6, 7, 8, 9, 10, 11, 12 },   // 2133
    { 0, 1, 2, 3, 4, 5, 0xff, 0xff, 6, 7, 8, 9, 10, 11, 12, 13 },   // 3133
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 0xff, 4, 5, 6, 7, 8, 9, 10, 11 },   // 0233
    { 0, 1, 0xff, 0xff, 2, 3, 4, 0xff, 5, 6, 7, 8, 9, 10, 11, 12 },   // 1233
    { 0, 1, 2, 0xff, 3, 4, 5, 0xff, 6, 7, 8, 9, 10, 11, 12, 13 },   // 2233
    { 0, 1, 2, 3, 4, 5, 6, 0xff, 7, 8, 9, 10, 11, 12, 13, 14 },   // 3233
    { 0, 0xff, 0xff, 0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 },   // 0333
    { 0, 1, 0xff, 0xff, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 },   // 1333
    { 0, 1, 2, 0xff, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },   // 2333
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },   // 3333
};
// clang-format on

constexpr uint8_t length_lut[256]{
    4,  5,  6,  7,  5,  6,  7,  8,  6,  7,  8,  9,  7,  8,  9,  10, 5,  6,  7,  8,  6,  7,  8,  9,
    7,  8,  9,  10, 8,  9,  10, 11, 6,  7,  8,  9,  7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12,
    7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12, 10, 11, 12, 13, 5,  6,  7,  8,  6,  7,  8,  9,
    7,  8,  9,  10, 8,  9,  10, 11, 6,  7,  8,  9,  7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12,
    7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12, 10, 11, 12, 13, 8,  9,  10, 11, 9,  10, 11, 12,
    10, 11, 12, 13, 11, 12, 13, 14, 6,  7,  8,  9,  7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12,
    7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12, 10, 11, 12, 13, 8,  9,  10, 11, 9,  10, 11, 12,
    10, 11, 12, 13, 11, 12, 13, 14, 9,  10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
    7,  8,  9,  10, 8,  9,  10, 11, 9,  10, 11, 12, 10, 11, 12, 13, 8,  9,  10, 11, 9,  10, 11, 12,
    10, 11, 12, 13, 11, 12, 13, 14, 9,  10, 11, 12, 10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15,
    10, 11, 12, 13, 11, 12, 13, 14, 12, 13, 14, 15, 13, 14, 15, 16,
};

} // namespace

} // namespace detail

template <std::size_t BlockLen>
struct streamvbyte_base {
  using integer_type = std::uint32_t;
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t STEPLEN = 8;
  // 2bit per integer
  static constexpr std::size_t CTRLLEN = ( BLOCKLEN >> 2 ) + ( ( ( BLOCKLEN & 3 ) + 3 ) >> 2 );

  // safe overapproximation of the compressed size per block ( in bytes )
  static constexpr std::size_t estimate_compressed_size() noexcept {
    static_assert( BLOCKLEN % 4 == 0 );
    return CTRLLEN + BLOCKLEN * sizeof( integer_type );
  }
};

template <typename Delta, std::size_t BlockLen>
struct streamvbyte_i128 : public streamvbyte_base<BlockLen> {
  using base_type = streamvbyte_base<BlockLen>;
  using integer_type = typename base_type::integer_type;

  static inline std::size_t encode( integer_type const* const in, std::byte* const out ) noexcept {
    return encode( in, base_type::BLOCKLEN, out );
  }

  // `n` needs to be a multiple of `STEPLEN`
  static inline std::size_t encode(
      integer_type const* const in, std::size_t const n, std::byte* const out ) noexcept {

    auto const blocklen = std::min( n, base_type::BLOCKLEN );

    auto const mask_01 = _mm_set1_epi8( 0x01 );
    auto const mask_7f00 = _mm_set1_epi16( 0x7f00 );

    auto* out_p = out;
    auto const* const end = in + blocklen;
    auto const* p = in;
    __m128i prev = _mm_set1_epi32( 0 );
    while( p < end ) {
      auto const* in_v = reinterpret_cast<__m128i const*>( p );
      __m128i const x0_raw = _mm_loadu_si128( in_v );
      __m128i const x1_raw = _mm_loadu_si128( in_v + 1 );

      __m128i x0 = Delta::delta( x0_raw, prev );
      __m128i x1 = Delta::delta( x1_raw, x0_raw );
      prev = x1_raw;

      __m128i x2 = _mm_min_epu8( mask_01, x0 );
      __m128i x3 = _mm_min_epu8( mask_01, x1 );

      x2 = _mm_packus_epi16( x2, x3 );
      // convert 0x01ff to 0x0101
      x2 = _mm_min_epi16( x2, mask_01 );
      // convert: 0x0101 to 0x8001, 0xff01 to 0xffff
      x2 = _mm_adds_epu16( x2, mask_7f00 );

      auto const ctrl = static_cast<unsigned>( _mm_movemask_epi8( x2 ) );
      std::memcpy( out_p, &ctrl, 2 );
      out_p += 2;

      x2 = _mm_loadu_si128(
          reinterpret_cast<__m128i const*>( &detail::shuffle_lut[( ctrl << 4 ) & 0x03F0] ) );
      x3 = _mm_loadu_si128(
          reinterpret_cast<__m128i const*>( &detail::shuffle_lut[( ctrl >> 4 ) & 0x03F0] ) );

      x0 = _mm_shuffle_epi8( x0, x2 );
      x1 = _mm_shuffle_epi8( x1, x3 );

      _mm_storeu_si128( reinterpret_cast<__m128i*>( out_p ), x0 );
      out_p += detail::length_lut[ctrl & 0xff];
      _mm_storeu_si128( reinterpret_cast<__m128i*>( out_p ), x1 );
      out_p += detail::length_lut[ctrl >> 8];
      p += 8;
    }

    return static_cast<std::size_t>( out_p - out );
  }

  static inline std::size_t decode(
      std::byte const* const in, std::size_t const n, integer_type* const out ) noexcept {
    auto const* const out_end = reinterpret_cast<__m128i const*>( out + base_type::BLOCKLEN );
    auto* out_p = reinterpret_cast<__m128i*>( out );
    auto const* const end = in + n;
    auto const* p = in;
    __m128i prev = _mm_set1_epi32( 0 );
    while( p < end && out_p < out_end ) {
      auto const ctrl0 = static_cast<std::uint_fast8_t>( p[0] );
      auto const ctrl1 = static_cast<std::uint_fast8_t>( p[1] );
      auto const in0_v = reinterpret_cast<__m128i const*>( p + 2 );

      auto const x0_p = _mm_loadu_si128( in0_v );
      auto const* lut0 = reinterpret_cast<std::uint8_t const*>( &detail::decode_shuffle_lut[ctrl0] );
      auto const s0 = _mm_loadu_si128( reinterpret_cast<__m128i const*>( lut0 ) );
      auto const len0 = static_cast<unsigned>( lut0[12 + ( ctrl0 >> 6 )] ) + 1;
      auto const x0_raw = _mm_shuffle_epi8( x0_p, s0 );
      auto const x0 = Delta::undelta( x0_raw, prev );

      auto const in1_v = reinterpret_cast<__m128i const*>( p + 2 + len0 );
      auto const x1_p = _mm_loadu_si128( in1_v );
      auto const* lut1 = reinterpret_cast<std::uint8_t const*>( &detail::decode_shuffle_lut[ctrl1] );
      auto const s1 = _mm_loadu_si128( reinterpret_cast<__m128i const*>( lut1 ) );
      auto const len1 = static_cast<unsigned>( lut1[12 + ( ctrl1 >> 6 )] ) + 1;
      auto const x1 = Delta::undelta( _mm_shuffle_epi8( x1_p, s1 ), x0 );

      _mm_storeu_si128( out_p, x0 );
      _mm_storeu_si128( out_p + 1, x1 );

      prev = x1;
      out_p += 2;
      p += 2 + len0 + len1;
    }

    return static_cast<std::size_t>( p - in );
  }
};

struct svb128_i128 : streamvbyte_i128<delta::nodelta, 128> {};
struct svb128d1_i128 : streamvbyte_i128<delta::delta_regular, 128> {};
struct svb256d1_i128 : streamvbyte_i128<delta::delta_regular, 256> {};

} // namespace riot::streamvbyte
