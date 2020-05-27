// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libriot/compress-bitpack-simd-i128.hxx>
#include <libriot/compress-bitpack-simd-i256.hxx>
#include <libriot/compress-delta-simd.hxx>
#include <libriot/compress-integer.hxx>

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include <immintrin.h>

namespace riot::bitpack {

namespace detail {

inline std::size_t encode_ctrl(
    std::uint32_t const bits, std::uint32_t const sv, std::byte* const out ) noexcept {
  using b = std::byte;
  auto const sv_len = std::max( 1u, ( ( 32 - _lzcnt_u32( sv ) ) + 7 ) >> 3 );
  out[0] = b( ( ( sv_len - 1 ) << 6 ) | bits );
  switch( sv_len ) {
    case 1: out[1] = b( sv ); return 2;
    case 2:
      out[1] = b( sv >> 8 );
      out[2] = b( sv );
      return 3;
    case 3:
      out[1] = b( sv >> 16 );
      out[2] = b( sv >> 8 );
      out[3] = b( sv );
      return 4;
    case 4:
      out[1] = b( sv >> 24 );
      out[2] = b( sv >> 16 );
      out[3] = b( sv >> 8 );
      out[4] = b( sv );
      return 5;
    default: __builtin_unreachable();
  }
}

inline std::pair<std::uint_fast8_t, std::uint_fast8_t> decode_ctrl(
    std::byte const* const p, std::size_t const n, std::uint32_t& sv ) noexcept {
  using u = std::uint32_t;
  auto const tag = static_cast<std::uint_fast8_t>( p[0] );
  auto const bits = tag & ( ( 1 << 6 ) - 1 );
  auto const len = ( tag >> 6 ) & 3;
  if( len + 2 > n ) { return { 0, 0 }; }
  // clang-format off
  switch( len ) {
    case 0b00: sv = u( p[1] ); break;
    case 0b01: sv = u( p[1] ) << 8 | u( p[2] ); break;
    case 0b10: sv = u( p[1] ) << 16 | u( p[2] ) << 8 | u( p[3] ); break;
    case 0b11: sv = u( p[1] ) << 24 | u( p[2] ) << 16 | u( p[3] ) << 8 | u( p[4] ); break;
    default: sv = 0;
  }
  // clang-format on
  return { bits, len + 2 };
}

} // namespace detail

template <std::size_t StepLen, std::size_t BlockLen>
struct bitpack_base {
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t STEPLEN = StepLen;
  using integer_type = std::uint32_t;
  static constexpr std::size_t CTRLLEN = 1 + 4;
  static_assert( BLOCKLEN % STEPLEN == 0 );

  // safe overapproximation of the compressed size per block ( in bytes )
  static constexpr std::size_t estimate_compressed_size() noexcept {
    return CTRLLEN + BLOCKLEN * sizeof( integer_type );
  }
};

struct bitpack_delta_i256 : public bitpack_base<8, 256> {
  using delta = delta::delta_i256<integer_type, BLOCKLEN>;
  static_assert( delta::STEPLEN == STEPLEN );

  static inline std::size_t encode(
      integer_type const* const in, std::size_t const n, std::byte* const out ) noexcept {

    integer_type tmp[BLOCKLEN] = { 0 };
    auto const blocklen = std::min( n, BLOCKLEN );
    auto const sv = in[0];
    auto const bits = delta::delta_maxbits( in, BLOCKLEN, tmp, sv );
    auto const ctrl_len = detail::encode_ctrl( bits, sv, out );

    __m256i* const out_v = reinterpret_cast<__m256i*>( out + ctrl_len );
    switch( bits ) {
      case 0: detail::pack256_0( tmp, out_v ); break;
      case 1: detail::pack256_1( tmp, out_v ); break;
      case 2: detail::pack256_2( tmp, out_v ); break;
      case 3: detail::pack256_3( tmp, out_v ); break;
      case 4: detail::pack256_4( tmp, out_v ); break;
      case 5: detail::pack256_5( tmp, out_v ); break;
      case 6: detail::pack256_6( tmp, out_v ); break;
      case 7: detail::pack256_7( tmp, out_v ); break;
      case 8: detail::pack256_8( tmp, out_v ); break;
      case 9: detail::pack256_9( tmp, out_v ); break;
      case 10: detail::pack256_10( tmp, out_v ); break;
      case 11: detail::pack256_11( tmp, out_v ); break;
      case 12: detail::pack256_12( tmp, out_v ); break;
      case 13: detail::pack256_13( tmp, out_v ); break;
      case 14: detail::pack256_14( tmp, out_v ); break;
      case 15: detail::pack256_15( tmp, out_v ); break;
      case 16: detail::pack256_16( tmp, out_v ); break;
      case 17: detail::pack256_17( tmp, out_v ); break;
      case 18: detail::pack256_18( tmp, out_v ); break;
      case 19: detail::pack256_19( tmp, out_v ); break;
      case 20: detail::pack256_20( tmp, out_v ); break;
      case 21: detail::pack256_21( tmp, out_v ); break;
      case 22: detail::pack256_22( tmp, out_v ); break;
      case 23: detail::pack256_23( tmp, out_v ); break;
      case 24: detail::pack256_24( tmp, out_v ); break;
      case 25: detail::pack256_25( tmp, out_v ); break;
      case 26: detail::pack256_26( tmp, out_v ); break;
      case 27: detail::pack256_27( tmp, out_v ); break;
      case 28: detail::pack256_28( tmp, out_v ); break;
      case 29: detail::pack256_29( tmp, out_v ); break;
      case 30: detail::pack256_30( tmp, out_v ); break;
      case 31: detail::pack256_31( tmp, out_v ); break;
      case 32: detail::pack256_32( tmp, out_v ); break;
      default: __builtin_unreachable();
    }

    return ctrl_len + ( ( ( ( bits * blocklen ) + 255 ) >> 8 ) << 5 );
  }

  static inline void decode_block(
      std::byte const* const in, integer_type* const out, unsigned const bits ) noexcept {

    __m256i const* in_v = reinterpret_cast<__m256i const*>( in );
    switch( bits ) {
      case 0: detail::unpack256_0( in_v, out ); break;
      case 1: detail::unpack256_1( in_v, out ); break;
      case 2: detail::unpack256_2( in_v, out ); break;
      case 3: detail::unpack256_3( in_v, out ); break;
      case 4: detail::unpack256_4( in_v, out ); break;
      case 5: detail::unpack256_5( in_v, out ); break;
      case 6: detail::unpack256_6( in_v, out ); break;
      case 7: detail::unpack256_7( in_v, out ); break;
      case 8: detail::unpack256_8( in_v, out ); break;
      case 9: detail::unpack256_9( in_v, out ); break;
      case 10: detail::unpack256_10( in_v, out ); break;
      case 11: detail::unpack256_11( in_v, out ); break;
      case 12: detail::unpack256_12( in_v, out ); break;
      case 13: detail::unpack256_13( in_v, out ); break;
      case 14: detail::unpack256_14( in_v, out ); break;
      case 15: detail::unpack256_15( in_v, out ); break;
      case 16: detail::unpack256_16( in_v, out ); break;
      case 17: detail::unpack256_17( in_v, out ); break;
      case 18: detail::unpack256_18( in_v, out ); break;
      case 19: detail::unpack256_19( in_v, out ); break;
      case 20: detail::unpack256_20( in_v, out ); break;
      case 21: detail::unpack256_21( in_v, out ); break;
      case 22: detail::unpack256_22( in_v, out ); break;
      case 23: detail::unpack256_23( in_v, out ); break;
      case 24: detail::unpack256_24( in_v, out ); break;
      case 25: detail::unpack256_25( in_v, out ); break;
      case 26: detail::unpack256_26( in_v, out ); break;
      case 27: detail::unpack256_27( in_v, out ); break;
      case 28: detail::unpack256_28( in_v, out ); break;
      case 29: detail::unpack256_29( in_v, out ); break;
      case 30: detail::unpack256_30( in_v, out ); break;
      case 31: detail::unpack256_31( in_v, out ); break;
      case 32: detail::unpack256_32( in_v, out ); break;
      default: break;
    }
  }

  static inline void decode_short(
      std::byte const* const in,
      std::size_t const n,
      integer_type* const out,
      unsigned const bits ) noexcept {
    std::byte tmp[estimate_compressed_size()] = { std::byte( 0 ) };
    std::memcpy( tmp, in, n );
    decode_block( tmp, out, bits );
  }

  static inline std::size_t decode(
      std::byte const* const in, std::size_t const n, integer_type* const out ) noexcept {
    integer_type x;
    auto [bits, ctrl_len] = detail::decode_ctrl( in, n, x );
    auto const n_in = ctrl_len + ( ( ( ( bits * BLOCKLEN ) + 255 ) >> 8 ) << 5 );
    if( n < n_in ) {
      decode_short( in + ctrl_len, n - ctrl_len, out, bits );
      delta::undelta( out, BLOCKLEN, x );
      return ctrl_len + ( ( ( ( bits * ( n - ctrl_len ) / STEPLEN ) + 255 ) >> 8 ) << 5 );
    }
    decode_block( in + ctrl_len, out, bits );
    delta::undelta( out, BLOCKLEN, x );
    return n_in;
  }
};

struct bitpack_delta_i128 : bitpack_base<4, 128> {
  using delta = delta::delta_i256<integer_type, BLOCKLEN>;
  static_assert( BLOCKLEN % delta::STEPLEN == 0 );

  static inline std::size_t encode(
      integer_type const* const in, std::size_t n, std::byte* const out ) noexcept {

    integer_type tmp[BLOCKLEN] = { 0 };
    auto const blocklen = std::min( n, BLOCKLEN );
    auto const sv = in[0];
    auto const bits = delta::delta_maxbits( in, BLOCKLEN, tmp, sv );
    auto const ctrl_len = detail::encode_ctrl( bits, sv, out );

    __m128i* const out_v = reinterpret_cast<__m128i*>( out + ctrl_len );
    switch( bits ) {
      case 0: detail::pack128_0( tmp, out_v ); break;
      case 1: detail::pack128_1( tmp, out_v ); break;
      case 2: detail::pack128_2( tmp, out_v ); break;
      case 3: detail::pack128_3( tmp, out_v ); break;
      case 4: detail::pack128_4( tmp, out_v ); break;
      case 5: detail::pack128_5( tmp, out_v ); break;
      case 6: detail::pack128_6( tmp, out_v ); break;
      case 7: detail::pack128_7( tmp, out_v ); break;
      case 8: detail::pack128_8( tmp, out_v ); break;
      case 9: detail::pack128_9( tmp, out_v ); break;
      case 10: detail::pack128_10( tmp, out_v ); break;
      case 11: detail::pack128_11( tmp, out_v ); break;
      case 12: detail::pack128_12( tmp, out_v ); break;
      case 13: detail::pack128_13( tmp, out_v ); break;
      case 14: detail::pack128_14( tmp, out_v ); break;
      case 15: detail::pack128_15( tmp, out_v ); break;
      case 16: detail::pack128_16( tmp, out_v ); break;
      case 17: detail::pack128_17( tmp, out_v ); break;
      case 18: detail::pack128_18( tmp, out_v ); break;
      case 19: detail::pack128_19( tmp, out_v ); break;
      case 20: detail::pack128_20( tmp, out_v ); break;
      case 21: detail::pack128_21( tmp, out_v ); break;
      case 22: detail::pack128_22( tmp, out_v ); break;
      case 23: detail::pack128_23( tmp, out_v ); break;
      case 24: detail::pack128_24( tmp, out_v ); break;
      case 25: detail::pack128_25( tmp, out_v ); break;
      case 26: detail::pack128_26( tmp, out_v ); break;
      case 27: detail::pack128_27( tmp, out_v ); break;
      case 28: detail::pack128_28( tmp, out_v ); break;
      case 29: detail::pack128_29( tmp, out_v ); break;
      case 30: detail::pack128_30( tmp, out_v ); break;
      case 31: detail::pack128_31( tmp, out_v ); break;
      case 32: detail::pack128_32( tmp, out_v ); break;
      default: __builtin_unreachable();
    }

    return ctrl_len + ( ( ( ( bits * blocklen ) + 127 ) >> 7 ) << 4 );
  }

  static inline void decode_block(
      std::byte const* const in, integer_type* const out, unsigned const bits ) noexcept {

    __m128i const* in_v = reinterpret_cast<__m128i const*>( in );
    switch( bits ) {
      case 0: detail::unpack128_0( in_v, out ); break;
      case 1: detail::unpack128_1( in_v, out ); break;
      case 2: detail::unpack128_2( in_v, out ); break;
      case 3: detail::unpack128_3( in_v, out ); break;
      case 4: detail::unpack128_4( in_v, out ); break;
      case 5: detail::unpack128_5( in_v, out ); break;
      case 6: detail::unpack128_6( in_v, out ); break;
      case 7: detail::unpack128_7( in_v, out ); break;
      case 8: detail::unpack128_8( in_v, out ); break;
      case 9: detail::unpack128_9( in_v, out ); break;
      case 10: detail::unpack128_10( in_v, out ); break;
      case 11: detail::unpack128_11( in_v, out ); break;
      case 12: detail::unpack128_12( in_v, out ); break;
      case 13: detail::unpack128_13( in_v, out ); break;
      case 14: detail::unpack128_14( in_v, out ); break;
      case 15: detail::unpack128_15( in_v, out ); break;
      case 16: detail::unpack128_16( in_v, out ); break;
      case 17: detail::unpack128_17( in_v, out ); break;
      case 18: detail::unpack128_18( in_v, out ); break;
      case 19: detail::unpack128_19( in_v, out ); break;
      case 20: detail::unpack128_20( in_v, out ); break;
      case 21: detail::unpack128_21( in_v, out ); break;
      case 22: detail::unpack128_22( in_v, out ); break;
      case 23: detail::unpack128_23( in_v, out ); break;
      case 24: detail::unpack128_24( in_v, out ); break;
      case 25: detail::unpack128_25( in_v, out ); break;
      case 26: detail::unpack128_26( in_v, out ); break;
      case 27: detail::unpack128_27( in_v, out ); break;
      case 28: detail::unpack128_28( in_v, out ); break;
      case 29: detail::unpack128_29( in_v, out ); break;
      case 30: detail::unpack128_30( in_v, out ); break;
      case 31: detail::unpack128_31( in_v, out ); break;
      case 32: detail::unpack128_32( in_v, out ); break;
      default: break;
    }
  }

  static inline void decode_short(
      std::byte const* const in,
      std::size_t const n,
      integer_type* const out,
      unsigned const bits ) noexcept {
    std::byte tmp[estimate_compressed_size()] = { std::byte( 0 ) };
    std::memcpy( tmp, in, n );
    decode_block( tmp, out, bits );
  }

  static inline std::size_t decode(
      std::byte const* const in, std::size_t const n, integer_type* const out ) noexcept {
    integer_type x;
    auto [bits, ctrl_len] = detail::decode_ctrl( in, n, x );
    auto const n_in = ctrl_len + ( ( ( ( bits * BLOCKLEN ) + 127 ) >> 7 ) << 4 );
    if( n < n_in ) {
      decode_short( in + ctrl_len, n - ctrl_len, out, bits );
      delta::undelta( out, BLOCKLEN, x );
      return ctrl_len + ( ( ( ( bits * ( n - ctrl_len ) / STEPLEN ) + 127 ) >> 7 ) << 4 );
    }
    decode_block( in + ctrl_len, out, bits );
    delta::undelta( out, BLOCKLEN, x );
    return n_in;
  }
};

using bp256d1 = bitpack_delta_i256;
using bp128d1 = bitpack_delta_i128;

} // namespace riot::bitpack
