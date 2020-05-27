// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libriot/compress-delta-simd.hxx>
#include <libriot/compress-vbyte.hxx>

#include <cstddef>

namespace riot::streamvqb {

template <std::size_t BlockLen>
struct streamvqb_base {
  using integer_type = std::uint32_t;
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t STEPLEN = 8;
  static constexpr std::size_t CTRLLEN = 3;
  static constexpr std::size_t estimate_compressed_size() noexcept {
    return CTRLLEN + BLOCKLEN * sizeof( integer_type );
  }
};

template <std::size_t BlockLen>
struct streamvqb : public streamvqb_base<BlockLen> {
  using self = streamvqb_base<BlockLen>;
  using integer_type = typename self::integer_type;
  using delta = delta::delta_regular;

  // `n` must be multiple of `STEPLEN`
  static inline std::size_t encode(
      integer_type const* const in, std::size_t const n, std::byte* const out ) noexcept {
    auto const blocklen = std::min( n, self::BLOCKLEN );
    std::byte buf[16];
    auto n_sv = riot::vbkey::encode( buf, in[0] );

    auto* p = out;
    auto const* in_p = reinterpret_cast<__m128i const*>( in );
    auto const* const end = reinterpret_cast<__m128i const*>( in + blocklen );
    __m128i sv = _mm_set1_epi32( static_cast<int>( in[0] ) );
    while( in_p < end ) {

      auto const v0_raw = _mm_loadu_si128( in_p );
      auto const v1_raw = _mm_loadu_si128( in_p + 1 );

      auto const v0 = delta::delta( v0_raw, sv );
      auto const v1 = delta::delta( v1_raw, v0_raw );

      auto const v0o = riot::delta::hor_epi32( v0 );
      auto const v1o = riot::delta::hor_epi32( v1 );
      auto const len0 = ( ( 32 - _lzcnt_u32( v0o | 1 ) ) + 1 ) >> 1;
      auto const len1 = ( ( 32 - _lzcnt_u32( v1o | 1 ) ) + 1 ) >> 1;

      auto bits = ( ( len0 << 1 ) << 2 ) + ( ( len1 << 1 ) << 2 );
      auto bytes = ( bits + 7 ) >> 3;

      in_p += 2;
      sv = v1_raw;
      p += 1 /*ctrl*/ + bytes;
    }

    return n_sv + static_cast<std::size_t>( p - out );
  }

  static inline void decode(
      std::byte const* const, std::size_t const, integer_type* const ) noexcept {
    std::abort();
  }
};

using svq128d1 = streamvqb<128>;

} // namespace riot::streamvqb
