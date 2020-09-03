// SPDX-License-Identifier: 0BSD

#pragma once

// @see ( with modifications ):
//   - <https://github.com/miloyip/dtoa-benchmark/milo/dtoa_milo.h>
//   - <https://github.com/Tencent/rapidjson/blob/master/include/rapidjson/internal/dtoa.h>

#include <libunclassified/format-itoa.hxx>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#define UINT64_C2( h, l ) ( ( static_cast<uint64_t>( h ) << 32 ) | static_cast<uint64_t>( l ) )

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified::format {

using char_t = char;

namespace detail {

union d2i {
  double const d;
  std::uint64_t u64;
};

static_assert( sizeof( double ) == sizeof( std::uint64_t ) );

inline bool dirty_compare_exact( double const x, double const y ) {
  d2i x_{ x };
  d2i y_{ y };

  return x_.u64 == y_.u64;
}

struct DiyFp {
  DiyFp() noexcept {}

  DiyFp( uint64_t f_, int e_ ) noexcept : f( f_ ), e( e_ ) {}

  DiyFp( double d ) noexcept {
    d2i u{ d };

    int biased_e = ( u.u64 & kDpExponentMask ) >> kDpSignificandSize;
    uint64_t significand = ( u.u64 & kDpSignificandMask );
    if( biased_e != 0 ) {
      f = significand + kDpHiddenBit;
      e = biased_e - kDpExponentBias;
    } else {
      f = significand;
      e = kDpMinExponent + 1;
    }
  }

  DiyFp operator-( const DiyFp& rhs ) const noexcept {
    assert( e == rhs.e );
    assert( f >= rhs.f );
    return DiyFp( f - rhs.f, e );
  }

  DiyFp operator*( const DiyFp& rhs ) const noexcept {
    unsigned __int128 p = static_cast<unsigned __int128>( f ) *
        static_cast<unsigned __int128>( rhs.f );
    uint64_t h = p >> 64;
    uint64_t l = static_cast<uint64_t>( p );
    if( l & ( uint64_t( 1 ) << 63 ) ) // rounding
      h++;
    return DiyFp( h, e + rhs.e + 64 );
  }

  DiyFp Normalize() const noexcept {
    int s = __builtin_clzll( f );
    return DiyFp( f << s, e - s );
  }

  DiyFp NormalizeBoundary() const {
    auto index = __builtin_clzll( f );
    return DiyFp( f << index, e - index );
#if 0
    DiyFp res = *this;
    while( !( res.f & ( kDpHiddenBit << 1 ) ) ) {
      res.f <<= 1;
      res.e--;
    }
    res.f <<= ( kDiySignificandSize - kDpSignificandSize - 2 );
    res.e = res.e - ( kDiySignificandSize - kDpSignificandSize - 2 );
    return res;
#endif
  }

  void NormalizedBoundaries( DiyFp* minus, DiyFp* plus ) const noexcept {
    DiyFp pl = DiyFp( ( f << 1 ) + 1, e - 1 ).NormalizeBoundary();
    DiyFp mi = ( f == kDpHiddenBit ) ? DiyFp( ( f << 2 ) - 1, e - 2 ) : DiyFp( ( f << 1 ) - 1, e - 1 );
    mi.f <<= mi.e - pl.e;
    mi.e = pl.e;
    *plus = pl;
    *minus = mi;
  }

  static constexpr int kDiySignificandSize = 64;
  static constexpr int kDpSignificandSize = 52;
  static constexpr int kDpExponentBias = 0x3FF + kDpSignificandSize;
  static constexpr int kDpMinExponent = -kDpExponentBias;
  static constexpr uint64_t kDpExponentMask = UINT64_C2( 0x7FF00000, 0x00000000 );
  static constexpr uint64_t kDpSignificandMask = UINT64_C2( 0x000FFFFF, 0xFFFFFFFF );
  static constexpr uint64_t kDpHiddenBit = UINT64_C2( 0x00100000, 0x00000000 );

  uint64_t f;
  int e;
};

inline DiyFp GetCachedPower( int e, int* K ) noexcept {
  // 10^-348, 10^-340, ..., 10^340
  static const uint64_t kCachedPowers_F[] = {
      UINT64_C2( 0xfa8fd5a0, 0x081c0288 ), UINT64_C2( 0xbaaee17f, 0xa23ebf76 ),
      UINT64_C2( 0x8b16fb20, 0x3055ac76 ), UINT64_C2( 0xcf42894a, 0x5dce35ea ),
      UINT64_C2( 0x9a6bb0aa, 0x55653b2d ), UINT64_C2( 0xe61acf03, 0x3d1a45df ),
      UINT64_C2( 0xab70fe17, 0xc79ac6ca ), UINT64_C2( 0xff77b1fc, 0xbebcdc4f ),
      UINT64_C2( 0xbe5691ef, 0x416bd60c ), UINT64_C2( 0x8dd01fad, 0x907ffc3c ),
      UINT64_C2( 0xd3515c28, 0x31559a83 ), UINT64_C2( 0x9d71ac8f, 0xada6c9b5 ),
      UINT64_C2( 0xea9c2277, 0x23ee8bcb ), UINT64_C2( 0xaecc4991, 0x4078536d ),
      UINT64_C2( 0x823c1279, 0x5db6ce57 ), UINT64_C2( 0xc2109436, 0x4dfb5637 ),
      UINT64_C2( 0x9096ea6f, 0x3848984f ), UINT64_C2( 0xd77485cb, 0x25823ac7 ),
      UINT64_C2( 0xa086cfcd, 0x97bf97f4 ), UINT64_C2( 0xef340a98, 0x172aace5 ),
      UINT64_C2( 0xb23867fb, 0x2a35b28e ), UINT64_C2( 0x84c8d4df, 0xd2c63f3b ),
      UINT64_C2( 0xc5dd4427, 0x1ad3cdba ), UINT64_C2( 0x936b9fce, 0xbb25c996 ),
      UINT64_C2( 0xdbac6c24, 0x7d62a584 ), UINT64_C2( 0xa3ab6658, 0x0d5fdaf6 ),
      UINT64_C2( 0xf3e2f893, 0xdec3f126 ), UINT64_C2( 0xb5b5ada8, 0xaaff80b8 ),
      UINT64_C2( 0x87625f05, 0x6c7c4a8b ), UINT64_C2( 0xc9bcff60, 0x34c13053 ),
      UINT64_C2( 0x964e858c, 0x91ba2655 ), UINT64_C2( 0xdff97724, 0x70297ebd ),
      UINT64_C2( 0xa6dfbd9f, 0xb8e5b88f ), UINT64_C2( 0xf8a95fcf, 0x88747d94 ),
      UINT64_C2( 0xb9447093, 0x8fa89bcf ), UINT64_C2( 0x8a08f0f8, 0xbf0f156b ),
      UINT64_C2( 0xcdb02555, 0x653131b6 ), UINT64_C2( 0x993fe2c6, 0xd07b7fac ),
      UINT64_C2( 0xe45c10c4, 0x2a2b3b06 ), UINT64_C2( 0xaa242499, 0x697392d3 ),
      UINT64_C2( 0xfd87b5f2, 0x8300ca0e ), UINT64_C2( 0xbce50864, 0x92111aeb ),
      UINT64_C2( 0x8cbccc09, 0x6f5088cc ), UINT64_C2( 0xd1b71758, 0xe219652c ),
      UINT64_C2( 0x9c400000, 0x00000000 ), UINT64_C2( 0xe8d4a510, 0x00000000 ),
      UINT64_C2( 0xad78ebc5, 0xac620000 ), UINT64_C2( 0x813f3978, 0xf8940984 ),
      UINT64_C2( 0xc097ce7b, 0xc90715b3 ), UINT64_C2( 0x8f7e32ce, 0x7bea5c70 ),
      UINT64_C2( 0xd5d238a4, 0xabe98068 ), UINT64_C2( 0x9f4f2726, 0x179a2245 ),
      UINT64_C2( 0xed63a231, 0xd4c4fb27 ), UINT64_C2( 0xb0de6538, 0x8cc8ada8 ),
      UINT64_C2( 0x83c7088e, 0x1aab65db ), UINT64_C2( 0xc45d1df9, 0x42711d9a ),
      UINT64_C2( 0x924d692c, 0xa61be758 ), UINT64_C2( 0xda01ee64, 0x1a708dea ),
      UINT64_C2( 0xa26da399, 0x9aef774a ), UINT64_C2( 0xf209787b, 0xb47d6b85 ),
      UINT64_C2( 0xb454e4a1, 0x79dd1877 ), UINT64_C2( 0x865b8692, 0x5b9bc5c2 ),
      UINT64_C2( 0xc83553c5, 0xc8965d3d ), UINT64_C2( 0x952ab45c, 0xfa97a0b3 ),
      UINT64_C2( 0xde469fbd, 0x99a05fe3 ), UINT64_C2( 0xa59bc234, 0xdb398c25 ),
      UINT64_C2( 0xf6c69a72, 0xa3989f5c ), UINT64_C2( 0xb7dcbf53, 0x54e9bece ),
      UINT64_C2( 0x88fcf317, 0xf22241e2 ), UINT64_C2( 0xcc20ce9b, 0xd35c78a5 ),
      UINT64_C2( 0x98165af3, 0x7b2153df ), UINT64_C2( 0xe2a0b5dc, 0x971f303a ),
      UINT64_C2( 0xa8d9d153, 0x5ce3b396 ), UINT64_C2( 0xfb9b7cd9, 0xa4a7443c ),
      UINT64_C2( 0xbb764c4c, 0xa7a44410 ), UINT64_C2( 0x8bab8eef, 0xb6409c1a ),
      UINT64_C2( 0xd01fef10, 0xa657842c ), UINT64_C2( 0x9b10a4e5, 0xe9913129 ),
      UINT64_C2( 0xe7109bfb, 0xa19c0c9d ), UINT64_C2( 0xac2820d9, 0x623bf429 ),
      UINT64_C2( 0x80444b5e, 0x7aa7cf85 ), UINT64_C2( 0xbf21e440, 0x03acdd2d ),
      UINT64_C2( 0x8e679c2f, 0x5e44ff8f ), UINT64_C2( 0xd433179d, 0x9c8cb841 ),
      UINT64_C2( 0x9e19db92, 0xb4e31ba9 ), UINT64_C2( 0xeb96bf6e, 0xbadf77d9 ),
      UINT64_C2( 0xaf87023b, 0x9bf0ee6b ),
  };
  static const int16_t kCachedPowers_E[] = {
      -1220, -1193, -1166, -1140, -1113, -1087, -1060, -1034, -1007, -980, -954, -927, -901,
      -874,  -847,  -821,  -794,  -768,  -741,  -715,  -688,  -661,  -635, -608, -582, -555,
      -529,  -502,  -475,  -449,  -422,  -396,  -369,  -343,  -316,  -289, -263, -236, -210,
      -183,  -157,  -130,  -103,  -77,   -50,   -24,   3,     30,    56,   83,   109,  136,
      162,   189,   216,   242,   269,   295,   322,   348,   375,   402,  428,  455,  481,
      508,   534,   561,   588,   614,   641,   667,   694,   720,   747,  774,  800,  827,
      853,   880,   907,   933,   960,   986,   1013,  1039,  1066,
  };

  //int k = static_cast<int>(ceil((-61 - e) * 0.30102999566398114)) + 374;
  double dk = ( -61 - e ) * 0.30102999566398114 +
      347; // dk must be positive, so can do ceiling in positive
  int k = static_cast<int>( dk );
  if( not dirty_compare_exact( k, dk ) ) k++;

  unsigned index = static_cast<unsigned>( ( k >> 3 ) + 1 );
  *K = -( -348 + static_cast<int>( index << 3 ) ); // decimal exponent no need lookup table

  assert( index < sizeof( kCachedPowers_F ) / sizeof( kCachedPowers_F[0] ) );
  return DiyFp( kCachedPowers_F[index], kCachedPowers_E[index] );
}

inline void GrisuRound( char_t* buffer, int len, uint64_t delta, uint64_t rest, uint64_t ten_kappa,
                        uint64_t wp_w ) noexcept {
  while( rest < wp_w && delta - rest >= ten_kappa &&
         ( rest + ten_kappa < wp_w || /// closer
           wp_w - rest > rest + ten_kappa - wp_w ) ) {
    buffer[len - 1]--;
    rest += ten_kappa;
  }
}

inline void DigitGen( const DiyFp& W, const DiyFp& Mp, uint64_t delta, char_t* buffer, int* len,
                      int* K ) noexcept {
  static const uint32_t kPow10[] = {
      1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000,
  };
  const DiyFp one( uint64_t( 1 ) << -Mp.e, Mp.e );
  const DiyFp wp_w = Mp - W;
  uint32_t p1 = static_cast<uint32_t>( Mp.f >> -one.e );
  uint64_t p2 = Mp.f & ( one.f - 1 );
  int kappa = static_cast<int>( unclassified::format::detail::ilog10u32( p1 ) );
  *len = 0;

  while( kappa > 0 ) {
    uint32_t d;
    switch( kappa ) {
      case 10:
        d = p1 / 1000000000;
        p1 %= 1000000000;
        break;
      case 9:
        d = p1 / 100000000;
        p1 %= 100000000;
        break;
      case 8:
        d = p1 / 10000000;
        p1 %= 10000000;
        break;
      case 7:
        d = p1 / 1000000;
        p1 %= 1000000;
        break;
      case 6:
        d = p1 / 100000;
        p1 %= 100000;
        break;
      case 5:
        d = p1 / 10000;
        p1 %= 10000;
        break;
      case 4:
        d = p1 / 1000;
        p1 %= 1000;
        break;
      case 3:
        d = p1 / 100;
        p1 %= 100;
        break;
      case 2:
        d = p1 / 10;
        p1 %= 10;
        break;
      case 1:
        d = p1;
        p1 = 0;
        break;
      default: __builtin_unreachable();
    }
    if( d || *len ) buffer[( *len )++] = '0' + static_cast<char>( d );
    kappa--;
    uint64_t tmp = ( static_cast<uint64_t>( p1 ) << -one.e ) + p2;
    if( tmp <= delta ) {
      *K += kappa;
      GrisuRound( buffer, *len, delta, tmp, static_cast<uint64_t>( kPow10[kappa] ) << -one.e, wp_w.f );
      return;
    }
  }

  // kappa = 0
  for( ;; ) {
    p2 *= 10;
    delta *= 10;
    char d = static_cast<char>( p2 >> -one.e );
    if( d || *len ) buffer[( *len )++] = '0' + d;
    p2 &= one.f - 1;
    kappa--;
    if( p2 < delta ) {
      *K += kappa;
      GrisuRound( buffer, *len, delta, p2, one.f, wp_w.f * kPow10[-kappa] );
      return;
    }
  }
}

inline void Grisu2( double value, char_t* buffer, int* length, int* K ) noexcept {
  const DiyFp v( value );
  DiyFp w_m, w_p;
  v.NormalizedBoundaries( &w_m, &w_p );

  const DiyFp c_mk = GetCachedPower( w_p.e, K );
  const DiyFp W = v.Normalize() * c_mk;
  DiyFp Wp = w_p * c_mk;
  DiyFp Wm = w_m * c_mk;
  Wm.f++;
  Wp.f--;
  DigitGen( W, Wp, Wp.f - Wm.f, buffer, length, K );
}

inline char_t* WriteExponent( int K, char_t* buffer ) noexcept {
  if( K < 0 ) {
    *buffer++ = '-';
    K = -K;
  }

  if( K >= 100 ) {
    *buffer++ = '0' + static_cast<char>( K / 100 );
    K %= 100;
    unsigned i = static_cast<unsigned>( K * 2 );
    *buffer++ = unclassified::format::detail::digits_table[i];
    *buffer++ = unclassified::format::detail::digits_table[i + 1];
  } else if( K >= 10 ) {
    unsigned i = static_cast<unsigned>( K * 2 );
    *buffer++ = unclassified::format::detail::digits_table[i];
    *buffer++ = unclassified::format::detail::digits_table[i + 1];
  } else
    *buffer++ = '0' + static_cast<char>( K );

  *buffer = '\0';
  return buffer;
}

inline char_t* Prettify( char_t* buffer, int length, int k ) noexcept {
  const int kk = length + k; // 10^(kk-1) <= v < 10^kk

  if( length <= kk && kk <= 21 ) {
    // 1234e7 -> 12340000000
    for( int i = length; i < kk; i++ ) buffer[i] = '0';
    buffer[kk] = '.';
    buffer[kk + 1] = '0';
    return &buffer[kk + 2];
  } else if( 0 < kk && kk <= 21 ) {
    // 1234e-2 -> 12.34
    memmove( &buffer[kk + 1], &buffer[kk], static_cast<std::size_t>( length - kk ) );
    buffer[kk] = '.';
    return &buffer[length + 1];
  } else if( -6 < kk && kk <= 0 ) {
    // 1234e-6 -> 0.001234
    const int offset = 2 - kk;
    memmove( &buffer[offset], &buffer[0], static_cast<std::size_t>( length ) );
    buffer[0] = '0';
    buffer[1] = '.';
    for( int i = 2; i < offset; i++ ) buffer[i] = '0';
    return &buffer[length + offset];
  } else if( length == 1 ) {
    // 1e30
    buffer[1] = 'e';
    return WriteExponent( kk - 1, &buffer[2] );
  } else {
    // 1234e30 -> 1.234e33
    memmove( &buffer[2], &buffer[1], static_cast<std::size_t>( length - 1 ) );
    buffer[1] = '.';
    buffer[length + 1] = 'e';
    return WriteExponent( kk - 1, &buffer[0 + length + 2] );
  }
}
} // namespace detail

inline unsigned dtoa( double value, char_t* const out ) noexcept {
  char_t* buffer = out;
  if( std::isnan( value ) ) {
    buffer[0] = 'n';
    buffer[1] = 'a';
    buffer[2] = 'n';
    return 3;
  } else if( std::isinf( value ) ) {
    buffer[0] = 'i';
    buffer[1] = 'n';
    buffer[2] = 'f';
    return 3;
  } else if( value == .0 ) {
    buffer[0] = '0';
    buffer[1] = '.';
    buffer[2] = '0';
    return 3;
  } else {
    if( value < .0 ) {
      *buffer++ = '-';
      value = -value;
    }
    int length, K;
    detail::Grisu2( value, buffer, &length, &K );
    char_t* p = detail::Prettify( buffer, length, K );
    return static_cast<unsigned>( p - out );
  }
}

} // namespace unclassified::format

#ifdef __clang__
#pragma clang diagnostic pop
#endif
