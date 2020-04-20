// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/compression/streamvqb-simd.hxx>

#include <array>
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

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
