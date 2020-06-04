// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

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
            equal_to( "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "00000000000000000000000000000000000000000000000000000000" ) );
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
            equal_to( "0000fe000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "00000000000000000000000000000000000000000000000000000000"

                      ) );
  } );

  test( "svb128d1_i128: compress all zeros", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );
    auto n = svb128d1::encode( in.data(), out.data() );
    expect( n, equal_to( svb128d1::CTRLLEN + svb128d1::BLOCKLEN ) );
    expect( hexify( out, n ),
            equal_to( "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "00000000000000000000000000000000000000000000000000000000" ) );
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
            equal_to( "0000fe010000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000"
                      "00000000000000000000000000000000000000000000000000000000" ) );
  } );

  test( "svb128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<svb128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svb128d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = svb128d1::encode( in.data(), svb128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 277u ) );
    expect( hexify( out, n ),
            equal_to( "555582021e012b01400687043003be01cf0815513804660973065fd6023e8f0211"
                      "051555e4086d05d206f63e0190091806170715517b0364075b09b11e013dc7052d"
                      "055555c4092a03ee064307b305a0035502a20255548203f4070808290271ec0213"
                      "01aa085554f6085906cc03c502cf3403cc046d0555542f037305a10446079b0507"
                      "500499055555e60787035901840215045d040409c10955552506140735062d0593"
                      "019509490750055555550344086b018208c6010b07b006d80255557f079d04ce05"
                      "9b0409091707f701cd065515560131035d051a07530531024b03df555444077c05"
                      "6908be08f4560790086c065555f40537092804f10351029a09b6078a0415558703"
                      "5305d9027224046406f9079d09" ) );
    auto n_dec = svb128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "svb128d1: encode BLOCKLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> in;
    std::array<std::byte, svb128d1::estimate_compressed_size()> out;
    std::array<svb128d1::integer_type, svb128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<svb128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < svb128d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + svb128d1::STEPLEN, svb128d1::BLOCKLEN - svb128d1::STEPLEN,
                 in[svb128d1::STEPLEN - 1] );
    auto n = svb128d1::encode( in.data(), svb128d1::STEPLEN, out.data() );
    expect( n, equal_to( 18u ) );
    expect( hexify( out, n ), equal_to( "555582021e012b01400687043003be01cf08" ) );
    auto n_dec = svb128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
