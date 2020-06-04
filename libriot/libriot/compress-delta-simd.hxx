// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libriot/compress-integer.hxx>

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include <immintrin.h>

namespace riot::delta {

constexpr std::size_t BLOCKLEN = 256;
constexpr std::size_t STEPLEN = 8;

inline std::uint32_t maxbits256_epi32( __m256i const x ) noexcept {
  __m256i const _tmp1 = _mm256_or_si256( _mm256_srli_si256( x, 8 ), x );
  __m256i const _tmp2 = _mm256_or_si256( _mm256_srli_si256( _tmp1, 4 ), _tmp1 );
  std::uint32_t const ans1 = static_cast<std::uint32_t>( _mm256_extract_epi32( _tmp2, 0 ) );
  std::uint32_t const ans2 = static_cast<std::uint32_t>( _mm256_extract_epi32( _tmp2, 4 ) );
  std::uint32_t const ans = std::max( ans1, ans2 );
  return 32 - _lzcnt_u32( ans );
}

// thx stackoverflow:
//   count leading zero bits for each element in AVX2 vector, emulate _mm256_lzcnt_epi32
//
inline __m256i lzcnt256_epi32( __m256i const x ) noexcept {
  // prevent value from being rounded up to the next power of two
  auto v = _mm256_andnot_si256( _mm256_srli_epi32( x, 8 ), x ); // keep 8 MSB
  v = _mm256_castps_si256( _mm256_cvtepi32_ps( v ) ); // convert an integer to float
  v = _mm256_srli_epi32( v, 23 ); // shift down the exponent
  v = _mm256_subs_epu16( _mm256_set1_epi32( 158 ), v ); // undo bias
  v = _mm256_min_epi16( v, _mm256_set1_epi32( 32 ) ); // clamp at 32
  return v;
}

// thx stackoverflow:
//   fastest method to calculate sum of all packed 32-bit integers using AVX512 or AVX2
//
// this general strategy is appropriate for all element types: float, double, and
// any size integer (except 8-bit where you use vpsadbw)
//
inline std::uint32_t hsum_epi32_avx( __m128i x ) {
  // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
  __m128i hi64 = _mm_unpackhi_epi64( x, x );
  __m128i sum64 = _mm_add_epi32( hi64, x );
  __m128i hi32 = _mm_shuffle_epi32( sum64, _MM_SHUFFLE( 2, 3, 0, 1 ) ); // Swap the low two elements
  __m128i sum32 = _mm_add_epi32( sum64, hi32 );
  return static_cast<std::uint32_t>( _mm_cvtsi128_si32( sum32 ) ); // movd
}

inline std::uint32_t hsum_8x32( __m256i v ) {
  __m128i sum128 = _mm_add_epi32(
      _mm256_castsi256_si128( v ),
      _mm256_extracti128_si256( v, 1 ) ); // silly GCC uses a longer AXV512VL instruction :/
  return hsum_epi32_avx( sum128 );
}

inline std::uint32_t hsum_16x32( __m512i v ) {
  __m256i sum256 = _mm256_add_epi32(
      _mm512_castsi512_si256( v ), // low half
      _mm512_extracti64x4_epi64( v, 1 ) ); // high half.  AVX512F.  32x8 version is AVX512DQ
  return hsum_8x32( sum256 );
}

inline std::uint32_t hor_epi32( __m128i const x ) noexcept {
  // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
  __m128i hi64 = _mm_unpackhi_epi64( x, x );
  __m128i op64 = _mm_or_si128( hi64, x );
  // Swap the low two elements
  __m128i hi32 = _mm_shuffle_epi32( op64, _MM_SHUFFLE( 2, 3, 0, 1 ) );
  __m128i op32 = _mm_or_si128( op64, hi32 );
  return static_cast<std::uint32_t>( _mm_cvtsi128_si32( op32 ) );
}

inline std::uint32_t maxbits256_epi32( const uint32_t* begin, std::size_t const n ) noexcept {
  auto const aligned = integer::align_up<STEPLEN>( n );
  auto const blocklen = std::min( aligned, BLOCKLEN );
  __m256i const* p = reinterpret_cast<__m256i const*>( begin );
  __m256i const* const end = reinterpret_cast<__m256i const*>( begin + blocklen );
  __m256i x = _mm256_lddqu_si256( p++ );
  while( p < end ) {
    __m256i const x0 = _mm256_lddqu_si256( p );
    x = _mm256_or_si256( x, x0 );
    p++;
  }
  return maxbits256_epi32( x );
}

struct delta_max4d {
  // the idea is due to N. Kurz
  static inline __m128i undelta( __m128i const x, __m128i const prev ) noexcept {
    return _mm_add_epi32( x, _mm_shuffle_epi32( prev, 0xff ) );
  }
  static inline __m128i delta( __m128i const x, __m128i const prev ) noexcept {
    return _mm_sub_epi32( x, _mm_shuffle_epi32( prev, 0xff ) );
  }
};

struct delta_regular {
  static inline __m128i undelta( __m128i const x, __m128i const prev ) noexcept {
    __m128i const _tmp1 = _mm_add_epi32( _mm_slli_si128( x, 8 ), x );
    __m128i const _tmp2 = _mm_add_epi32( _mm_slli_si128( _tmp1, 4 ), _tmp1 );
    return _mm_add_epi32( _tmp2, _mm_shuffle_epi32( prev, 0xff ) );
  }

  static inline __m128i delta( __m128i const x, __m128i const prev ) noexcept {
    return _mm_sub_epi32( x, _mm_alignr_epi8( x, prev, 12 ) );
  }
};

struct nodelta {
  static inline __m128i undelta( __m128i const x, __m128i const ) noexcept { return x; }

  static inline __m128i delta( __m128i const x, __m128i const ) noexcept { return x; }
};

template <typename I, std::size_t BlockLen>
struct delta_i128 {
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t STEPLEN = 4;
  using integer_type = I;

  static_assert( std::is_integral_v<I> );
  static_assert( sizeof( I ) == 4 );
  static_assert( BlockLen % STEPLEN == 0 );

  static inline void delta( integer_type* const inout, std::size_t const n,
                            integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m128i* p = reinterpret_cast<__m128i*>( inout );
    __m128i const* const end = reinterpret_cast<__m128i const*>( inout + blocklen );
    __m128i prev = _mm_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m128i const x0_raw = _mm_lddqu_si128( p );
      __m128i const x0 = _mm_sub_epi32( x0_raw, _mm_alignr_epi8( x0_raw, prev, 12 ) );
      _mm_storeu_si128( p, x0 );
      prev = x0_raw;
      p++;
    }
  }

  static inline void delta( integer_type const* const in, std::size_t const n, integer_type* const out,
                            integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m128i* out_p = reinterpret_cast<__m128i*>( out );
    __m128i const* p = reinterpret_cast<__m128i const*>( in );
    __m128i const* const end = reinterpret_cast<__m128i const*>( in + blocklen );
    __m128i prev = _mm_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m128i const x0_raw = _mm_lddqu_si128( p );
      __m128i const x0 = _mm_sub_epi32( x0_raw, _mm_alignr_epi8( x0_raw, prev, 12 ) );
      _mm_storeu_si128( out_p, x0 );
      prev = x0_raw;
      p++;
      out_p++;
    }
  }

  static inline void undelta( integer_type* const inout, std::size_t const n,
                              integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m128i* p = reinterpret_cast<__m128i*>( inout );
    __m128i const* const end = reinterpret_cast<__m128i const*>( inout + blocklen );
    __m128i prev = _mm_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m128i const x0_raw = _mm_lddqu_si128( p );
      __m128i const p0 = _mm_add_epi32( _mm_slli_si128( x0_raw, 8 ), x0_raw );
      __m128i x0 = _mm_add_epi32( _mm_slli_si128( p0, 4 ), p0 );
      prev = _mm_add_epi32( x0, _mm_shuffle_epi32( prev, 0xff ) );
      _mm_storeu_si128( p, prev );
      p++;
    }
  }
};

template <typename I, std::size_t BlockLen>
struct delta_i256 {
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t STEPLEN = 8;
  using integer_type = I;

  static_assert( std::is_integral_v<I> );
  static_assert( sizeof( I ) == 4 );
  static_assert( BlockLen % STEPLEN == 0 );

  static inline void delta( integer_type* const inout, std::size_t const n,
                            integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m256i* p = reinterpret_cast<__m256i*>( inout );
    __m256i const* const end = reinterpret_cast<__m256i const*>( inout + blocklen );
    __m256i prev = _mm256_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m256i const x0_raw = _mm256_lddqu_si256( p );
      __m256i const x0 = _mm256_sub_epi32(
          x0_raw,
          _mm256_alignr_epi8( x0_raw, _mm256_permute2f128_si256( prev, x0_raw, 2 << 4 | 1 ), 12 ) );
      _mm256_storeu_si256( p, x0 );
      prev = x0_raw;
      p++;
    }
  }

  static inline void delta( integer_type const* const in, std::size_t const n, integer_type* const out,
                            integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m256i* out_p = reinterpret_cast<__m256i*>( out );
    __m256i const* p = reinterpret_cast<__m256i const*>( in );
    __m256i const* const end = reinterpret_cast<__m256i const*>( in + blocklen );
    __m256i prev = _mm256_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m256i const x0_raw = _mm256_lddqu_si256( p );
      __m256i const x0 = _mm256_sub_epi32(
          x0_raw,
          _mm256_alignr_epi8( x0_raw, _mm256_permute2f128_si256( prev, x0_raw, 2 << 4 | 1 ), 12 ) );
      _mm256_storeu_si256( out_p, x0 );
      prev = x0_raw;
      out_p++;
      p++;
    }
  }

  static inline auto delta_maxbits( integer_type const* const in, std::size_t const n,
                                    integer_type* const out, integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m256i* out_p = reinterpret_cast<__m256i*>( out );
    __m256i const* p = reinterpret_cast<__m256i const*>( in );
    __m256i const* const end = reinterpret_cast<__m256i const*>( in + blocklen );
    __m256i prev = _mm256_set1_epi32( static_cast<int>( start ) );
    __m256i max = _mm256_setzero_si256();
    while( p < end ) {
      __m256i const x0_raw = _mm256_lddqu_si256( p );
      __m256i const x0 = _mm256_sub_epi32(
          x0_raw,
          _mm256_alignr_epi8( x0_raw, _mm256_permute2f128_si256( prev, x0_raw, 2 << 4 | 1 ), 12 ) );
      _mm256_storeu_si256( out_p, x0 );
      max = _mm256_or_si256( max, x0 );
      prev = x0_raw;
      out_p++;
      p++;
    }
    return maxbits256_epi32( max );
  }

  static inline void undelta( integer_type* const inout, std::size_t const n,
                              integer_type const start = 0 ) noexcept {
    auto const aligned = integer::align_up<STEPLEN>( n );
    auto const blocklen = std::min( aligned, BLOCKLEN );
    __m128i* p = reinterpret_cast<__m128i*>( inout );
    __m128i const* const end = reinterpret_cast<__m128i const*>( inout + blocklen );
    __m128i prev = _mm_set1_epi32( static_cast<int>( start ) );
    while( p < end ) {
      __m128i const x0_raw = _mm_lddqu_si128( p );
      __m128i const p0 = _mm_add_epi32( _mm_slli_si128( x0_raw, 8 ), x0_raw );
      __m128i x0 = _mm_add_epi32( _mm_slli_si128( p0, 4 ), p0 );
      prev = _mm_add_epi32( x0, _mm_shuffle_epi32( prev, 0xff ) );
      _mm_storeu_si128( p, prev );
      p++;
    }
  }

  // TODO: implement avx2 prefix sum
  //static inline void undelta(
  //    integer_type* const inout, std::size_t const n,
  //    integer_type const start = 0 ) noexcept {
  //  auto const aligned = integer::align_up<STEPLEN>( n );
  //  auto const blocklen = std::min( aligned, BLOCKLEN );
  //  __m256i* p = reinterpret_cast<__m256i*>( inout );
  //  __m256i const* const end = reinterpret_cast<__m256i const*>(
  //      inout + blocklen );
  //  __m256i prev = _mm256_set1_epi32( static_cast<int>( start ) );
  //  while( p < end ) {
  //    __m256i const x0_raw = _mm256_lddqu_si256( p );
  //    __m256i const p0 =
  //        _mm256_add_epi32( _mm256_slli_si256( x0_raw, 8 ), x0_raw );
  //    __m256i x0 = _mm256_add_epi32( _mm256_slli_si256( p0, 4 ), p0 );
  //    prev = _mm256_add_epi32( x0, _mm256_shuffle_epi32( prev, 0xff ) );
  //    _mm256_storeu_si256( p, prev );
  //    p++;
  //  }
  //}
};

} // namespace riot::delta
