// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/compress-delta-simd.hxx>

#include <array>
#include <random>

namespace {

std::uint32_t maxbits( std::uint32_t* p, std::size_t const n ) noexcept {
  std::uint32_t x = 0;
  for( unsigned i = 0; i < n; ++i ) {
    x = std::max( x, 32 - _lzcnt_u32( p[i] ) );
  }
  return x;
}

emptyspace::pest::suite basic( "delta compression basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "delta_i128 with delta/undelta of random data", []( auto& expect ) {
    using delta = riot::delta::delta_i128<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<delta::integer_type> random{ 60, 2500 };
    in[0] = tmp[0] = random( mt );
    for( std::size_t i = 1; i < delta::BLOCKLEN; ++i ) {
      in[i] = tmp[i] = in[i - 1] + random( mt );
    }
    expect( tmp == in, equal_to( true ) );
    delta::delta( tmp.data(), delta::BLOCKLEN );
    delta::undelta( tmp.data(), delta::BLOCKLEN );
    expect( tmp == in, equal_to( true ) );
  } );

  test( "delta_i128: check that last delta value", []( auto& expect ) {
    using delta = riot::delta::delta_i128<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    in.fill( 0xff );
    delta::delta( in.data(), delta::BLOCKLEN, tmp.data() );
    expect( tmp[0], equal_to( 0xffu ) );
    expect( tmp[tmp.size() - 1], equal_to( 0u ) );
  } );

  test( "delta_i128: check that delta( i, i - 1 ) == 1", []( auto& expect ) {
    using delta = riot::delta::delta_i128<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    in.fill( 0x10 );
    for( unsigned i = 1; i < in.size(); ++i ) {
      in[i] = in[i - 1] + 1;
    }
    delta::delta( in.data(), delta::BLOCKLEN, tmp.data() );
    for( unsigned i = 1; i < in.size(); ++i ) {
      expect( tmp[i], equal_to( 1u ) );
    }
    expect( tmp[0], equal_to( 0x10u ) );
  } );

  //--avx2--------------------------------------------------------------------

  test( "delta_i256: with delta/undelta of random data", []( auto& expect ) {
    using delta = riot::delta::delta_i256<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<delta::integer_type> random{ 60, 2500 };
    in[0] = tmp[0] = random( mt );
    for( std::size_t i = 1; i < delta::BLOCKLEN; ++i ) {
      in[i] = tmp[i] = in[i - 1] + random( mt );
    }
    expect( tmp == in, equal_to( true ) );
    delta::delta( tmp.data(), delta::BLOCKLEN );
    delta::undelta( tmp.data(), delta::BLOCKLEN );
    expect( tmp == in, equal_to( true ) );
  } );

  test( "delta_i256: check last delta value", []( auto& expect ) {
    using delta = riot::delta::delta_i256<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    in.fill( 0xff );
    delta::delta( in.data(), delta::BLOCKLEN, tmp.data() );
    expect( tmp[0], equal_to( 0xffu ) );
    expect( tmp[tmp.size() - 1], equal_to( 0u ) );
  } );

  test( "delta_i256: check that delta( i, i - 1 ) == 1", []( auto& expect ) {
    using delta = riot::delta::delta_i256<std::uint32_t, 128>;
    std::array<delta::integer_type, delta::BLOCKLEN> in;
    std::array<delta::integer_type, delta::BLOCKLEN> tmp;
    in.fill( 0x10 );
    for( unsigned i = 1; i < in.size(); ++i ) {
      in[i] = in[i - 1] + 1;
    }
    delta::delta( in.data(), delta::BLOCKLEN, tmp.data() );
    for( unsigned i = 1; i < in.size(); ++i ) {
      expect( tmp[i], equal_to( 1u ) );
    }
    expect( tmp[0], equal_to( 0x10u ) );
  } );

  test( "mm_maxbits_epi32: verify against scalar impl", []( auto& expect ) {
    constexpr std::size_t BLOCKLEN = 256;
    using delta = riot::delta::delta_i256<std::uint32_t, BLOCKLEN>;
    std::array<std::uint32_t, BLOCKLEN> in;
    std::array<std::uint32_t, BLOCKLEN> tmp;
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<std::uint32_t> random{ 60, 2500 };
    in[0] = 0;
    for( std::size_t i = 1; i < in.size(); ++i ) {
      in[i] = in[i - 1] + random( mt );
    }

    // compute maxbits of input data
    auto const max_scalar = maxbits( in.data(), BLOCKLEN );
    auto const max_simd = riot::delta::maxbits256_epi32( in.data(), BLOCKLEN );
    expect( max_simd, equal_to( max_scalar ) );
    expect( max_simd, equal_to( 19u ) );

    // compute delta, then maxbits
    delta::delta( in.data(), BLOCKLEN, tmp.data() );
    auto max_scalar_delta = maxbits( tmp.data(), BLOCKLEN );
    auto max_simd_delta = riot::delta::maxbits256_epi32( tmp.data(), BLOCKLEN );
    expect( max_simd_delta, equal_to( max_scalar_delta ) );
    expect( max_simd_delta, equal_to( 12u ) );

    // compute fused delta/maxbits
    tmp.fill( 0xffffffffu );
    auto deltamax_simd = delta::delta_maxbits( in.data(), BLOCKLEN, tmp.data() );
    auto deltamax_scalar = maxbits( tmp.data(), BLOCKLEN );
    expect( deltamax_simd, equal_to( deltamax_scalar ) );
    expect( deltamax_simd, equal_to( 12u ) );
  } );

  test( "mm_maxbits_epi32: with start value", []( auto& expect ) {
    constexpr std::size_t BLOCKLEN = 256;
    using delta = riot::delta::delta_i256<std::uint32_t, BLOCKLEN>;
    std::array<std::uint32_t, BLOCKLEN> in;
    std::array<std::uint32_t, BLOCKLEN> tmp;
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<std::uint32_t> random{ 60, 2500 };
    in[0] = 0xffffffffu;
    for( std::size_t i = 1; i < in.size(); ++i ) {
      in[i] = in[i - 1] + random( mt );
    }

    // w/o start value
    delta::delta_maxbits( in.data(), BLOCKLEN, tmp.data() );
    auto max_scalar_delta = maxbits( tmp.data(), BLOCKLEN );
    auto max_simd_delta = riot::delta::maxbits256_epi32( tmp.data(), BLOCKLEN );
    expect( max_simd_delta, equal_to( max_scalar_delta ) );
    expect( max_simd_delta, equal_to( 32u ) );

    // w/ start value
    auto const sv = in[0];
    auto deltamax_simd = delta::delta_maxbits( in.data(), BLOCKLEN, tmp.data(), sv );
    auto deltamax_scalar = maxbits( tmp.data(), BLOCKLEN );
    expect( deltamax_simd, equal_to( deltamax_scalar ) );
    expect( deltamax_simd, equal_to( 12u ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
