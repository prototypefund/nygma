// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <array>

namespace nygma::toeplitz {

static constexpr std::size_t RSSKEYSZ = 40;
static constexpr std::size_t RSSHASHSZ = 32;

// @see:
//   - DragonFlyBSD/tools/tools/toeplitz/toeplitz.c

template <typename Key>
struct toeplitz_scalar_lut {
  static_assert(
      Key::key.size() >= RSSKEYSZ, "Key must be at least toeplitz::RSSKEYSZ long" );
  std::uint32_t _hash[RSSHASHSZ][256];

 public:
  constexpr toeplitz_scalar_lut() noexcept {
    for( unsigned i = 0; i < RSSHASHSZ; i++ ) {
      std::uint32_t kk[8] = { 0 };
      for( unsigned b = 0; b < 8; b++ ) {
        for( unsigned j = 0; j < 32; j++ ) {
          unsigned bit = ( i * 8 ) + b + j;
          std::uint8_t k = Key::key[bit / 8];
          unsigned shift = 8 - ( bit % 8 ) - 1;
          if( k & ( 1 << shift ) ) { kk[b] |= 1 << ( 31 - j ); }
        }
      }
      for( unsigned val = 0; val < 256; val++ ) {
        std::uint32_t res = 0;
        for( unsigned b = 0; b < 8; b++ ) {
          unsigned shift = 8 - b - 1;
          if( val & ( 1 << shift ) ) { res ^= kk[b]; }
        }
        _hash[i][val] = res;
      }
    }
  }

 private:
  inline std::uint32_t hash8( std::byte const* p ) const noexcept {
    std::uint32_t hash = 0;
    hash ^= _hash[0][static_cast<std::size_t>( p[0] )];
    hash ^= _hash[1][static_cast<std::size_t>( p[1] )];
    hash ^= _hash[2][static_cast<std::size_t>( p[2] )];
    hash ^= _hash[3][static_cast<std::size_t>( p[3] )];
    hash ^= _hash[4][static_cast<std::size_t>( p[4] )];
    hash ^= _hash[5][static_cast<std::size_t>( p[5] )];
    hash ^= _hash[6][static_cast<std::size_t>( p[6] )];
    hash ^= _hash[7][static_cast<std::size_t>( p[7] )];
    return hash;
  }

  inline std::uint32_t hash32( std::byte const* p ) const noexcept {
    std::uint32_t hash = 0;
    hash ^= _hash[0][static_cast<std::size_t>( p[0] )];
    hash ^= _hash[1][static_cast<std::size_t>( p[1] )];
    hash ^= _hash[2][static_cast<std::size_t>( p[2] )];
    hash ^= _hash[3][static_cast<std::size_t>( p[3] )];
    hash ^= _hash[4][static_cast<std::size_t>( p[4] )];
    hash ^= _hash[5][static_cast<std::size_t>( p[5] )];
    hash ^= _hash[6][static_cast<std::size_t>( p[6] )];
    hash ^= _hash[7][static_cast<std::size_t>( p[7] )];
    hash ^= _hash[8][static_cast<std::size_t>( p[8] )];
    hash ^= _hash[9][static_cast<std::size_t>( p[9] )];
    hash ^= _hash[10][static_cast<std::size_t>( p[10] )];
    hash ^= _hash[11][static_cast<std::size_t>( p[11] )];
    hash ^= _hash[12][static_cast<std::size_t>( p[12] )];
    hash ^= _hash[13][static_cast<std::size_t>( p[13] )];
    hash ^= _hash[14][static_cast<std::size_t>( p[14] )];
    hash ^= _hash[15][static_cast<std::size_t>( p[15] )];
    hash ^= _hash[16][static_cast<std::size_t>( p[16] )];
    hash ^= _hash[17][static_cast<std::size_t>( p[17] )];
    hash ^= _hash[18][static_cast<std::size_t>( p[18] )];
    hash ^= _hash[19][static_cast<std::size_t>( p[19] )];
    hash ^= _hash[20][static_cast<std::size_t>( p[20] )];
    hash ^= _hash[21][static_cast<std::size_t>( p[21] )];
    hash ^= _hash[22][static_cast<std::size_t>( p[22] )];
    hash ^= _hash[23][static_cast<std::size_t>( p[23] )];
    hash ^= _hash[24][static_cast<std::size_t>( p[24] )];
    hash ^= _hash[25][static_cast<std::size_t>( p[25] )];
    hash ^= _hash[26][static_cast<std::size_t>( p[26] )];
    hash ^= _hash[27][static_cast<std::size_t>( p[27] )];
    hash ^= _hash[28][static_cast<std::size_t>( p[28] )];
    hash ^= _hash[29][static_cast<std::size_t>( p[29] )];
    hash ^= _hash[30][static_cast<std::size_t>( p[30] )];
    hash ^= _hash[31][static_cast<std::size_t>( p[31] )];
    return hash;
  }

 public:
  template <std::size_t N>
  constexpr std::uint32_t hash( std::byte const* const p ) const noexcept {
    static_assert( N == 8 || N == 32, "N must be either 8 bytes (ipv4) or 32 bytes (ipv6)" );
    if constexpr( N == 8 ) {
      return hash8( p );
    } else if constexpr( N == 32 ) {
      return hash32( p );
    }
  }
};

} // namespace nygma::analyze::toeplitz
