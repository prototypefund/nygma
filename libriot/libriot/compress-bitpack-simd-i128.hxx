// SPDX-License-Identifier: BSD-3-Clause

// @see:
//   - <https://github.com/lemire/simdcomp>

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <immintrin.h>

namespace riot::bitpack {

namespace detail {

namespace {

inline void pack128_0( std::uint32_t const*, __m128i* out ) noexcept {
  __m128i const zero = _mm_setzero_si128();
  _mm_storeu_si128( out, zero );
}

inline void pack128_1( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_2( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_3( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 3 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 3 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_5( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 5 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 5 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 5 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 5 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_6( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 6 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 6 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 6 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 6 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_7( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 7 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_9( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 9 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_10( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 10 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_11( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 11 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_12( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 12 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_13( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 13 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_14( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 14 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_15( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 15 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_17( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 17 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_18( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 18 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_19( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 19 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_20( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 20 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_21( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 21 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_22( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 22 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_23( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 23 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_24( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 24 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_25( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 25 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_26( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 26 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_27( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 27 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_28( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 28 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_29( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 29 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_30( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 30 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_31( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 31 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 30 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 29 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 27 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 26 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 25 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 23 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 22 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 21 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 19 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 18 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 17 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 15 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 14 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 13 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 11 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 10 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 9 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 7 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 6 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 5 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 3 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 2 ) );
  _mm_storeu_si128( out, OutReg );
  ++out;
  OutReg = _mm_srli_epi32( InReg, 31 - 1 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 1 ) );
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_32( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg = _mm_loadu_si128( in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
  ++out;
  InReg = _mm_loadu_si128( ++in );

  OutReg = InReg;
  _mm_storeu_si128( out, OutReg );
}

inline void pack128_4( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg;
  uint32_t outer;

  for( outer = 0; outer < 4; ++outer ) {
    InReg = _mm_loadu_si128( in );
    OutReg = InReg;

    InReg = _mm_loadu_si128( in + 1 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 4 ) );

    InReg = _mm_loadu_si128( in + 2 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );

    InReg = _mm_loadu_si128( in + 3 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 12 ) );

    InReg = _mm_loadu_si128( in + 4 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );

    InReg = _mm_loadu_si128( in + 5 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 20 ) );

    InReg = _mm_loadu_si128( in + 6 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );

    InReg = _mm_loadu_si128( in + 7 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 28 ) );
    _mm_storeu_si128( out, OutReg );
    ++out;

    in += 8;
  }
}

inline void pack128_8( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg;
  uint32_t outer;

  for( outer = 0; outer < 8; ++outer ) {
    InReg = _mm_loadu_si128( in );
    OutReg = InReg;

    InReg = _mm_loadu_si128( in + 1 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 8 ) );

    InReg = _mm_loadu_si128( in + 2 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );

    InReg = _mm_loadu_si128( in + 3 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 24 ) );
    _mm_storeu_si128( out, OutReg );
    ++out;

    in += 4;
  }
}

inline void pack128_16( std::uint32_t const* in_, __m128i* out ) noexcept {
  __m128i const* in = reinterpret_cast<__m128i const*>( in_ );
  __m128i OutReg;
  __m128i InReg;
  uint32_t outer;

  for( outer = 0; outer < 16; ++outer ) {
    InReg = _mm_loadu_si128( in );
    OutReg = InReg;

    InReg = _mm_loadu_si128( in + 1 );
    OutReg = _mm_or_si128( OutReg, _mm_slli_epi32( InReg, 16 ) );
    _mm_storeu_si128( out, OutReg );
    ++out;

    in += 2;
  }
}

inline void unpack128_0( [[maybe_unused]] __m128i const*, std::uint32_t* out_ ) noexcept {
  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i const zero = _mm_setzero_si128();
  _mm_storeu_si128( out, zero );
  _mm_storeu_si128( out + 1, zero );
  _mm_storeu_si128( out + 2, zero );
  _mm_storeu_si128( out + 3, zero );
  _mm_storeu_si128( out + 4, zero );
  _mm_storeu_si128( out + 5, zero );
  _mm_storeu_si128( out + 6, zero );
  _mm_storeu_si128( out + 7, zero );
  _mm_storeu_si128( out + 8, zero );
  _mm_storeu_si128( out + 9, zero );
  _mm_storeu_si128( out + 10, zero );
  _mm_storeu_si128( out + 11, zero );
  _mm_storeu_si128( out + 12, zero );
  _mm_storeu_si128( out + 13, zero );
  _mm_storeu_si128( out + 14, zero );
  _mm_storeu_si128( out + 15, zero );
  _mm_storeu_si128( out + 16, zero );
  _mm_storeu_si128( out + 17, zero );
  _mm_storeu_si128( out + 18, zero );
  _mm_storeu_si128( out + 19, zero );
  _mm_storeu_si128( out + 20, zero );
  _mm_storeu_si128( out + 21, zero );
  _mm_storeu_si128( out + 22, zero );
  _mm_storeu_si128( out + 23, zero );
  _mm_storeu_si128( out + 24, zero );
  _mm_storeu_si128( out + 25, zero );
  _mm_storeu_si128( out + 26, zero );
  _mm_storeu_si128( out + 27, zero );
  _mm_storeu_si128( out + 28, zero );
  _mm_storeu_si128( out + 29, zero );
  _mm_storeu_si128( out + 30, zero );
  _mm_storeu_si128( out + 31, zero );
}

inline void unpack128_1( __m128i const* in, uint32_t* out_ ) noexcept {
  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg1 = _mm_loadu_si128( in );
  __m128i InReg2 = InReg1;
  __m128i OutReg1, OutReg2, OutReg3, OutReg4;
  const __m128i mask = _mm_set1_epi32( 1 );

  uint32_t i, shift = 0;

  for( i = 0; i < 8; ++i ) {
    OutReg1 = _mm_and_si128( _mm_srli_epi32( InReg1, static_cast<int>( shift++ ) ), mask );
    OutReg2 = _mm_and_si128( _mm_srli_epi32( InReg2, static_cast<int>( shift++ ) ), mask );
    OutReg3 = _mm_and_si128( _mm_srli_epi32( InReg1, static_cast<int>( shift++ ) ), mask );
    OutReg4 = _mm_and_si128( _mm_srli_epi32( InReg2, static_cast<int>( shift++ ) ), mask );
    _mm_storeu_si128( out++, OutReg1 );
    _mm_storeu_si128( out++, OutReg2 );
    _mm_storeu_si128( out++, OutReg3 );
    _mm_storeu_si128( out++, OutReg4 );
  }
}

inline void unpack128_2( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 2 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 26 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 28 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 26 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 28 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_3( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 3 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 21 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 27 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 3 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 19 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 25 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 28 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 3 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 23 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 26 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_4( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 4 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_5( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 5 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 25 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 5 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 23 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 5 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 21 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 26 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 5 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 19 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 5 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_6( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 6 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 6 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 6 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 6 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 6 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_7( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 7 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 21 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 24 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 23 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 19 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 7 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_8( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 8 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_9( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 9 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 22 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 21 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 19 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 9 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_10( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 10 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 10 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_11( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 11 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 19 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 20 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 11 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_12( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 12 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 12 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_13( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 13 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 17 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 18 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 13 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_14( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 14 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 14 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_15( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 15 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 15 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 16 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 15 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_16( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 16 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_17( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 17 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 14 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 13 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 17 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_18( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 18 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 18 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_19( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 19 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 12 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 11 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 19 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_20( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 20 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 20 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_21( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 21 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 10 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 9 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 21 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_22( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 22 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 22 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_23( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 23 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 7 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 21 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 8 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 23 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 9 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_24( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 24 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 24 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_25( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 25 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 5 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 23 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 6 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 21 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 25 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 7 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_26( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 26 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 26 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_27( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 27 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 4 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 26 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 21 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 23 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 3 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 25 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 27 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 5 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_28( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 28 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 28 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_29( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 29 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 26 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 23 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 2 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 28 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 25 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( _mm_srli_epi32( InReg, 1 ), mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 27 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 21 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 29 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 3 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_30( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  __m128i const mask = _mm_set1_epi32( ( 1U << 30 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 28 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 26 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 2 );
  InReg = _mm_loadu_si128( ++in );

  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 28 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 26 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 30 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 2 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_31( __m128i const* in, uint32_t* out_ ) noexcept {

  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  __m128i InReg = _mm_loadu_si128( in );
  __m128i OutReg;
  const __m128i mask = _mm_set1_epi32( ( 1U << 31 ) - 1 );

  OutReg = _mm_and_si128( InReg, mask );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 31 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 30 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 30 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 29 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 29 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 28 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 28 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 27 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 27 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 26 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 26 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 25 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 25 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 24 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 24 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 23 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 23 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 22 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 22 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 21 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 21 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 20 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 20 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 19 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 19 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 18 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 18 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 17 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 17 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 16 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 16 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 15 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 15 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 14 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 14 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 13 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 13 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 12 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 12 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 11 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 11 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 10 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 10 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 9 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 9 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 8 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 8 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 7 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 7 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 6 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 6 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 5 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 5 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 4 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 4 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 3 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 3 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 2 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 2 );
  InReg = _mm_loadu_si128( ++in );

  OutReg = _mm_or_si128( OutReg, _mm_and_si128( _mm_slli_epi32( InReg, 31 - 1 ), mask ) );
  _mm_storeu_si128( out++, OutReg );

  OutReg = _mm_srli_epi32( InReg, 1 );
  _mm_storeu_si128( out++, OutReg );
}

inline void unpack128_32( __m128i const* in, std::uint32_t* const out_ ) noexcept {
  __m128i* out = reinterpret_cast<__m128i*>( out_ );
  for( std::uint32_t outer = 0; outer < 32; ++outer ) {
    _mm_storeu_si128( out++, _mm_loadu_si128( in++ ) );
  }
}

} // namespace
} // namespace detail
} // namespace emptyspace::bitpack
