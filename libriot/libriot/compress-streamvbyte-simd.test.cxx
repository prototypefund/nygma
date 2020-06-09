// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>
#include <pest/xoshiro.hxx>

#include <libriot/compress-streamvbyte-simd.hxx>

#include <array>
#include <random>

namespace {

emptyspace::pest::suite basic( "streamvbyte compression basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using svb128 = riot::streamvbyte::svb128_i128;
  using svb128d1 = riot::streamvbyte::svb128d1_i128;

  test( "svb128_i128: compress all zeros", []( auto& expect ) {
    std::array<svb128::integer_type, svb128::BLOCKLEN> in;
    std::array<std::byte, svb128::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );

    auto n = svb128::encode( in.data(), out.data() );
    expect( n, equal_to( svb128::CTRLLEN + svb128::BLOCKLEN ) );
    expect( hexify( out, n ),
            equal_to( "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000" ) );
  } );

  test( "svb128_i128: compress #1", []( auto& expect ) {
    std::array<svb128::integer_type, svb128::BLOCKLEN> in;
    std::array<std::byte, svb128::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = svb128::encode( in.data(), out.data() );
    expect( n, equal_to( svb128::CTRLLEN + svb128::BLOCKLEN ) );
    expect( hexify( out, n ),
            equal_to( "0000fe0000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000" ) );
  } );

  test( "svb128d1_i128: compress all zeros", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );
    auto n = svb128d1::encode( in.data(), out.data() );
    expect( n, equal_to( svb128d1::CTRLLEN + svb128d1::BLOCKLEN ) );
    expect( hexify( out, n ),
            equal_to( "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000" ) );
  } );

  test( "svb128d1_i128: compress 8", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = svb128d1::encode( in.data(), 8, out.data() );
    expect( n, equal_to( 10u ) );
    expect( hexify( out, n ), equal_to( "0000fe01000000000000" ) );
  } );

  test( "svb128d1_i128: compress", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = svb128d1::encode( in.data(), out.data() );
    expect( n, equal_to( svb128d1::CTRLLEN + svb128d1::BLOCKLEN ) );
    expect( hexify( out, n ),
            equal_to( "0000fe0100000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "0000" ) );
  } );

  test( "svb128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    std::uniform_int_distribution<svb128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svb128d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = svb128d1::encode( in.data(), svb128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 274u ) );
    expect( hexify( out, n ),
            equal_to( "55112b06ae02bc06e003a401626403945555fa02e3081b01c0044502fd01c502f0035555e301e60"
                      "4cd027f081e056f037405bb075554a90945013801b9016a510457031b025555c101fb080d05b004"
                      "290588068b01dd025455ea230306073b0192021706da018e045555660706090f03a80124020c02d"
                      "a06c305555033025408d00803085d426804f508555523042409120765056a07410486071a095555"
                      "4a0966026b0607079a0885079806df0545512d068b067b9f06bf02a3900994071555c3059f074d0"
                      "66a82030f031f08d6065555df070d027b09f201db019801bc0883041454f38106f1055c438505fe"
                      "03f5045055dedd4a03760763018007c10512055555f404fe0237059108ab021a0488092e04" ) );
    auto n_dec = svb128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "svb128d1: encode STEPLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    std::uniform_int_distribution<svb128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svb128d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + svb128d1::STEPLEN, svb128d1::BLOCKLEN - svb128d1::STEPLEN,
                 in[svb128d1::STEPLEN - 1] );
    auto n = svb128d1::encode( in.data(), svb128d1::STEPLEN, out.data() );
    expect( n, equal_to( 16u ) );
    expect( hexify( out, n ), equal_to( "55112b06ae02bc06e003a40162640394" ) );
    auto n_dec = svb128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    for( std::size_t i = 0; i < svb128d1::STEPLEN; ++i ) { expect( in[i] == dec[i] ); }
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
