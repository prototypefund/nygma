// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libriot/indexing/index-builder.hxx>
#include <libriot/indexing/index-serializer.hxx>

#include <libriot/compression/bitpack-simd.hxx>
#include <libriot/compression/streamvbyte-simd.hxx>
#include <libriot/compression/streamvqb-simd.hxx>

#include <array>
#include <vector>

namespace riot {

//--streamvbyte---------------------------------------------------------------

template <typename OStream>
struct svb128d1_serializer //
  : compressing_serializer<
        OStream,
        method::SVB128D1,
        streamvbyte::svb128d1_i128,
        method::SVB128D1,
        streamvbyte::svb128d1_i128> {};

template <typename OStream>
struct svb256d1_serializer //
  : compressing_serializer<
        OStream,
        method::SVB256D1,
        streamvbyte::svb256d1_i128,
        method::SVB256D1,
        streamvbyte::svb256d1_i128> {};

template <typename OStream>
svb128d1_serializer( OStream ) -> svb128d1_serializer<OStream>;

template <typename OStream>
svb256d1_serializer( OStream ) -> svb256d1_serializer<OStream>;

//--streamvqb------------------------------------------------------------------

template <typename OStream>
struct svq128d1_serializer //
  : compressing_serializer<
        OStream,
        method::SVQ4x0D1,
        streamvqb::svq128d1,
        method::SVQ4x0D1,
        streamvqb::svq128d1> {};

template <typename OStream>
svq128d1_serializer( OStream ) -> svq128d1_serializer<OStream>;

//--bitpack--------------------------------------------------------------------

template <typename OStream>
struct bp256d1_serializer //
  : compressing_serializer<
        OStream,
        method::BP256D1,
        bitpack::bp256d1,
        method::BP256D1,
        bitpack::bp256d1> {};

template <typename OStream>
bp256d1_serializer( OStream ) -> bp256d1_serializer<OStream>;

template <typename OStream>
struct bp128d1_serializer //
  : compressing_serializer<
        OStream,
        method::BP128D1,
        bitpack::bp128d1,
        method::BP128D1,
        bitpack::bp128d1> {};

template <typename OStream>
bp128d1_serializer( OStream ) -> bp128d1_serializer<OStream>;

} // namespace riot
