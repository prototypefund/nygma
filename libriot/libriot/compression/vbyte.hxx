// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libunclassified/bytestring.hxx>

#include <algorithm>
#include <cstdint>

#include <immintrin.h>

// a varint implementation that preserves lexicographic sort order
//   - encoded integers can still be using in kv-stores like leveldb/rocksdb
//   - prefix-varint encoding
//   - big endinan
//         5        0
// +--------'--------+
//  0.......'........  +0bits (1byte tot)
//  10......'........  +8bits (2byte tot)
//  110.....'........  +16bits (3byte tot)
//  1110....'........  +24bits (4byte tot)
//  ...
//  1111111'111111111  +64bits (9byte tot)

namespace riot::vbkey {

namespace unsafe = unclassified::unsafe;
using endianess = unclassified::endianess;
using byte_t = std::byte;

constexpr unsigned nencode( std::uint64_t const x ) noexcept {
  // clzll of `0` is undefined, so safe here
  unsigned bits = 63 - static_cast<unsigned>( __builtin_clzll( x | 1 ) );
  unsigned n = std::min( 1 + bits / 7, 9u );
  return n;
}

inline void encode( byte_t* const p, unsigned const n, std::uint64_t const x ) noexcept {
  using b = byte_t;
  static constexpr auto BE = endianess::BE;
  switch( n ) {
    case 1: p[0] = b( x ); break;
    case 2:
      p[0] = b( x >> 8 | 0b1000'0000 );
      p[1] = b( x );
      break;
    case 3:
      p[0] = b( x >> 16 | 0b1100'0000 );
      p[1] = b( x >> 8 );
      p[2] = b( x );
      break;
    case 4:
      p[0] = b( x >> 24 | 0b1110'0000 );
      unsafe::wr24<BE>( p + 1, static_cast<std::uint32_t>( x ) );
      break;
    case 5:
      p[0] = b( x >> 32 | 0b1111'0000 );
      unsafe::wr32<BE>( p + 1, static_cast<std::uint32_t>( x ) );
      break;
    case 6:
      p[0] = b( x >> 40 | 0b1111'1000 );
      unsafe::wr40<BE>( p + 1, x );
      break;
    case 7:
      p[0] = b( x >> 48 | 0b1111'1100 );
      unsafe::wr48<BE>( p + 1, x );
      break;
    case 8:
      p[0] = b( x >> 56 | 0b1111'1110 );
      unsafe::wr56<BE>( p + 1, x );
      break;
    case 9:
      p[0] = b( 0b1111'1111 );
      unsafe::wr64<BE>( p + 1, x );
      break;
    default: __builtin_unreachable();
  }
}

inline unsigned ndecode( byte_t const* const p ) noexcept {
  static_assert( sizeof( unsigned ) == 4 );
  byte_t tag = *p;
  unsigned n = static_cast<unsigned>(
      __builtin_clz( ( unsigned( ~tag ) << 24 ) | 0b0000'0000'1000'0000'0000'0000'0000'0000 ) );
  return n + 1;
}

inline std::uint64_t decode( byte_t* const p, unsigned const n ) noexcept {
  using u = std::uint64_t;
  using b = byte_t;
  static constexpr auto be = endianess::BE;
  switch( n ) {
    // clang-format off
    case 1: return u( p[0] );
    case 2: return u( p[0] & ~b( 0b1000'0000 ) ) << 8 | u( p[1] );
    case 3: return u( p[0] & ~b( 0b1100'0000 ) ) << 16 | unsafe::rd16<be>( p + 1 );
    case 4: return u( p[0] & ~b( 0b1110'0000 ) ) << 24 | unsafe::rd24<be>( p + 1 );
    case 5: return u( p[0] & ~b( 0b1111'0000 ) ) << 32 | unsafe::rd32<be>( p + 1 );
    case 6: return u( p[0] & ~b( 0b1111'1000 ) ) << 40 | unsafe::rd40<be>( p + 1 );
    case 7: return u( p[0] & ~b( 0b1111'1100 ) ) << 48 | unsafe::rd48<be>( p + 1 );
    case 8: return u( p[0] & ~b( 0b1111'1110 ) ) << 56 | unsafe::rd56<be>( p + 1 );
    case 9: return unsafe::rd64<be>( p + 1 );
    default: __builtin_unreachable();
      // clang-format on
  }
  return n;
}

template <std::size_t N>
inline unsigned encode( byte_t ( &p )[N], std::uint64_t const x ) noexcept {
  static_assert( N >= 9 );
  auto const n = nencode( x );
  encode( p, n, x );
  return n;
}

} // namespace riot::vbkey

namespace riot::vbyte {

namespace unsafe = unclassified::unsafe;
using endianess = unclassified::endianess;
using byte_t = std::byte;

inline unsigned nencode( std::uint32_t const x ) noexcept {
  return std::max( 1u, unsigned( 32 - _lzcnt_u32( x ) + 7 ) >> 3 );
}

inline unsigned encode( std::byte* out, unsigned n, std::uint32_t const x ) noexcept {
  using b = byte_t;
  switch( n ) {
    case 1: out[0] = b( x ); return 0b00;
    case 2:
      out[0] = b( x >> 8 );
      out[1] = b( x );
      return 0b01;
    case 3:
      out[0] = b( x >> 16 );
      out[1] = b( x >> 8 );
      out[2] = b( x );
      return 0b10;
    case 4:
      out[0] = b( x >> 24 );
      out[1] = b( x >> 16 );
      out[2] = b( x >> 8 );
      out[3] = b( x );
      return 0b11;
    default: __builtin_unreachable();
  }
}

inline std::uint8_t decode_tag( byte_t const* const p ) noexcept {
  static_assert( sizeof( unsigned ) == 4 );
  std::uint8_t tag = static_cast<std::uint8_t>( p[0] );
  return ( tag & 3 );
}

inline std::uint32_t decode( byte_t const* p, std::uint8_t const tag ) noexcept {
  using u = std::uint32_t;
  static constexpr auto BE = endianess::BE;
  switch( tag ) {
    case 0b00: return u( p[0] );
    case 0b01: return unsafe::rd16<BE>( p );
    case 0b10: return unsafe::rd24<BE>( p );
    case 0b11: return unsafe::rd32<BE>( p );
    default: __builtin_unreachable();
  }
}

inline std::pair<std::uint8_t, unsigned> encode( byte_t* p, std::uint32_t const x ) noexcept {
  auto const n = nencode( x );
  auto tag = encode( p, n, x );
  return { tag, n };
}

template <std::size_t N>
inline std::pair<std::uint8_t, unsigned> encode( byte_t ( &p )[N], std::uint32_t const x ) noexcept {
  static_assert( N >= 4 );
  auto const n = nencode( x );
  auto tag = encode( p, n, x );
  return { tag, n };
}

} // namespace emptyspace::vbyte
