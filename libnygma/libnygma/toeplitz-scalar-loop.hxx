// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <array>

namespace nygma::toeplitz {

// @see:
//   - github.com/scylladb/seastar/include/seastar/net/toeplitz.hh

template <typename Key>
struct toeplitz_scalar_loop {
  template <std::size_t N>
  inline std::uint32_t hash( std::byte const* data ) {
    static_assert( N == 8 || N == 32, "N must be either 8 bytes (ipv4) or 32 bytes (ipv6)" );
    // XXX: Perhaps an assertion about key length vs. data length?
    std::uint32_t v = ( Key::key[0] << 24 ) + ( Key::key[1] << 16 ) + ( Key::key[2] << 8 ) +
        Key::key[3];
    uint32_t hash = 0;
    for( unsigned int i = 0; i < N; i++ ) {
      for( unsigned int b = 0; b < 8; b++ ) {
        unsigned char const c = static_cast<unsigned char>( data[i] );
        if( c & ( 1 << ( 7 - b ) ) ) hash ^= v;
        v <<= 1;
        if( ( i + 4 ) < Key::key.size() && ( Key::key[i + 4] & ( 1 << ( 7 - b ) ) ) ) v |= 1;
      }
    }
    return hash;
  }
};

} // namespace nygma::toeplitz
