// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libriot/compress-streamvqb-simd.hxx>

#include <array>
#include <iomanip>
#include <random>

namespace {

emptyspace::pest::suite basic( "streamvqb compression suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using svq128d1 = riot::streamvqb::svq128d1;

  test( "svq128d1: compress all zeros", []( auto& expect ) {
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> in;
    std::array<std::byte, svq128d1::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );

    auto n = svq128d1::encode( in.data(), svq128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 112u ) );
    expect(
        hexify( out, n ),
        equal_to( "00000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "00000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000000000000000000000000000000" ) );
  } );

  test( "svq128d1: compress", []( auto& expect ) {
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> in;
    std::array<std::byte, svq128d1::estimate_compressed_size()> out;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = svq128d1::encode( in.data(), svq128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 112u ) );
    expect(
        hexify( out, n ),
        equal_to( "20000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "00000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                  "0000000000000000000000000000000000000000000000000000000000" ) );
  } );

  test( "svq128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> in;
    std::array<std::byte, svq128d1::estimate_compressed_size()> out;
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<svq128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svq128d1::BLOCKLEN; ++i ) {
      in[i] = in[i - 1] + random( mt );
    }
    auto n = svq128d1::encode( in.data(), svq128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 238u ) );
    expect( hexify( out, n ), equal_to( "" ) );
    //auto n_dec = svq128d1::decode( out.data(), n, dec.data() );
    //expect( n_dec, equal_to( n ) );
    //expect( in == dec, equal_to( true ) );
  } );
} );

std::ostream& operator<<( std::ostream& os, __m128 const in ) {
  alignas( 32 ) std::array<std::uint8_t, 16> v;
  _mm_store_si128( reinterpret_cast<__m128i*>( v.data() ), in );
  unsigned cnt = 0;
  std::cout << "| ";
  for( auto x = v.rbegin(); x != v.rend(); ++x ) {
    cnt++;
    os << std::hex << std::setw( 2 ) << std::setfill( '0' ) << int( *x )
       << ( ( cnt % 4 == 0 ) ? " | " : " " );
  }
  return os;
}
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

inline std::uint32_t hor_epi32( __m128i const x ) noexcept {
  // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
  __m128i hi64 = _mm_unpackhi_epi64( x, x );
  __m128i op64 = _mm_or_si128( hi64, x );
  // Swap the low two elements
  __m128i hi32 = _mm_shuffle_epi32( op64, _MM_SHUFFLE( 2, 3, 0, 1 ) );
  __m128i op32 = _mm_or_si128( op64, hi32 );
  return static_cast<std::uint32_t>( _mm_cvtsi128_si32( op32 ) );
}
template <std::size_t BlockLen>
struct streamvqb : public streamvqb_base<BlockLen> {
  using self = streamvqb_base<BlockLen>;
  using integer_type = typename self::integer_type;
  using delta = riot::delta::delta_regular;

  // `n` must be multiple of `STEPLEN`
  static inline std::size_t encode(
      integer_type const* const in, std::size_t const n, std::byte* const out ) noexcept {

    auto blocklen = std::min( n, self::BLOCKLEN );

    __m128i sv = _mm_set1_epi32( static_cast<int>( in[0] ) );
    std::cout << "sv =    " << sv << std::endl;

    std::byte buf[16];
    auto n_sv = riot::vbkey::encode( buf, in[0] );

    auto* p = out;
    auto* in_p = reinterpret_cast<__m128i const*>( in );
    for( unsigned k = 0; k < blocklen; k += 8 ) {

      auto const v0_raw = _mm_loadu_si128( in_p );
      auto const v1_raw = _mm_loadu_si128( in_p + 1 );

      std::cout << "raw0 =   " << v0_raw << std::endl;
      std::cout << "raw1 =   " << v1_raw << std::endl;

      auto const v0 = delta::delta( v0_raw, sv );
      auto const v1 = delta::delta( v1_raw, v0_raw );

      std::cout << "v0 = " << v0 << std::endl;
      std::cout << "v1 = " << v1 << std::endl;

      auto const v0o = hor_epi32( v0 );
      auto const v1o = hor_epi32( v1 );
      auto const len0 = ( ( 32 - _lzcnt_u32( v0o | 1 ) ) + 1 ) >> 1;
      auto const len1 = ( ( 32 - _lzcnt_u32( v1o | 1 ) ) + 1 ) >> 1;

      std::cout << "or0 =    " << std::hex << std::setw( 4 ) << std::setfill( '0' ) << v0o
                << std::endl;
      std::cout << "or1 =    " << std::hex << std::setw( 4 ) << std::setfill( '0' ) << v1o
                << std::endl;
      std::cout << "len0 =   " << len0 << std::endl;
      std::cout << "len1 =   " << len1 << std::endl;

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

template <std::size_t LzCnt>
__m128i pack64( __m128i const x ) {
  if constexpr( LzCnt % 4 == 0 ) {
    auto const perm = _mm_set_epi8( 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 10, 9, 8, 2, 1, 0 );
    auto const v0 = _mm_srli_epi64( x, LzCnt );
    auto const v1 = _mm_or_si128( v0, x );
    std::cout << "v1 = " << v1 << std::endl;
    auto const v2 = _mm_shuffle_epi8( v1, perm );
    return v2;
  } else {
    auto const v0 = _mm_srli_epi64( x, LzCnt );
    auto const v1 = _mm_or_si128( v0, x );
    auto const v2 = _mm_slli_epi64( v1, ( 32 - LzCnt ) * 2 );
    auto const v3 = _mm_srli_si128( v2, 8 );
    auto const v4 = _mm_or_si128( v1, v3 );
    return v4;
  }
}
using svq128d1 = streamvqb<128>;

emptyspace::pest::suite basic_ex( "streamvbyte experimental suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "svq128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> in;
    std::array<std::byte, svq128d1::estimate_compressed_size()> out;
    std::array<svq128d1::integer_type, svq128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<svq128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svq128d1::BLOCKLEN; ++i ) {
      in[i] = in[i - 1] + random( mt );
    }
    auto n = svq128d1::encode( in.data(), svq128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 238u ) );
    expect( hexify( out, n ), equal_to( "" ) );
    //auto n_dec = svq128d1::decode( out.data(), n, dec.data() );
    //expect( n_dec, equal_to( n ) );
    //expect( in == dec, equal_to( true ) );
  } );

  test( "simd tests", []( auto& expect ) {
    auto x0 = _mm_setr_epi32( 0x800, 0x801, 0x802, 0x803 );
    auto bits = 32 - _lzcnt_u32( 0x8ff );
    std::cout << "bits = " << bits << std::endl;
    std::cout << "x0 = " << x0 << std::endl;
    auto x1 = _mm_shufflelo_epi16( x0, _MM_SHUFFLE( 3, 1, 2, 0 ) );
    std::cout << "x1 = " << x1 << std::endl;
    auto x2 = _mm_srli_epi64( x0, 20 );
    std::cout << "x2 = " << x2 << std::endl;
    auto x3 = pack64<20>( x0 );
    std::cout << "x3 = " << x3 << std::endl;

    auto x4 = _mm_setr_epi32( 0x20, 0x21, 0x22, 0x23 );
    auto x5 = pack64<26>( x4 );

    std::cout << "x4 = " << x4 << std::endl;
    std::cout << "x5 = " << x5 << std::endl;
    //auto n_dec = svq128d1::decode( out.data(), n, dec.data() );
    //expect( n_dec, equal_to( n ) );
    //expect( in == dec, equal_to( true ) );
    expect( true, equal_to( true ) );
  } );
} );
} // namespace

int main() {
  basic( std::clog );
  basic_ex( std::clog );
  return EXIT_SUCCESS;
}
