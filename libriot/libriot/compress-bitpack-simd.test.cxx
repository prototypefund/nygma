// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libriot/compress-bitpack-simd.hxx>

#include <array>
#include <random>

namespace {

emptyspace::pest::suite basic( "bitpack compression suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using bp256d1 = riot::bitpack::bp256d1;
  using bp128d1 = riot::bitpack::bp128d1;

  test( "bp256d1: encode all zeros", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );
    auto n = bp256d1::encode( in.data(), bp256d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 2u ) );
    expect( hexify( out, n ), equal_to( "0000" ) );
  } );

  test( "bp256d1: enc/dec all zeros", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> tmp;
    in.fill( 0x0 );
    tmp.fill( 0xffffffff );
    out.fill( std::byte( 0xff ) );
    auto n = bp256d1::encode( in.data(), bp256d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 2u ) );
    expect( hexify( out, n ), equal_to( "0000" ) );
    auto n_decoded = bp256d1::decode( out.data(), n, tmp.data() );
    expect( n_decoded, equal_to( n ) );
    expect( in == tmp, equal_to( true ) );
  } );

  test( "bp128d1: encode all zeros", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    in.fill( 0x0 );
    out.fill( std::byte( 0xff ) );
    auto n = bp128d1::encode( in.data(), bp128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 2u ) );
    expect( hexify( out, n ), equal_to( "0000" ) );
  } );

  test( "bp128d1: enc/dec all zeros", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> tmp;
    in.fill( 0x0 );
    tmp.fill( 0xffffffff );
    out.fill( std::byte( 0xff ) );
    auto n = bp128d1::encode( in.data(), bp128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 2u ) );
    expect( hexify( out, n ), equal_to( "0000" ) );
    auto n_decoded = bp128d1::decode( out.data(), n, tmp.data() );
    expect( n_decoded, equal_to( n ) );
    expect( in == tmp, equal_to( true ) );
  } );

  test( "bp256d1: encode STEPLEN", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = bp256d1::encode( in.data(), bp256d1::STEPLEN, out.data() );
    expect( n, equal_to( 34u ) );
    expect( hexify( out, n ),
            equal_to( "01fe00000000010000000000000000000000000000000000000000000000"
                      "00000000" ) );
  } );

  test( "bp128d1: encode STEPLEN", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    in[0] = 0xfe;
    auto n = bp128d1::encode( in.data(), bp128d1::STEPLEN, out.data() );
    expect( n, equal_to( 18u ) );
    expect( hexify( out, n ), equal_to( "01fe00000000010000000000000000000000" ) );
  } );

  test( "bp256d1: enc/dec BLOCKLEN", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> dec;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    dec.fill( 0xffffffffu );
    in[0] = 0xfe;
    auto n = bp256d1::encode( in.data(), bp256d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 34u ) );
    expect( hexify( out, n ),
            equal_to( "01fe00000000010000000000000000000000000000000000000000000000"
                      "00000000" ) );
    auto n_dec = bp256d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp128d1: enc/dec BLOCKLEN", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> dec;
    in.fill( 0xff );
    out.fill( std::byte( 0xff ) );
    dec.fill( 0xffffffffu );
    in[0] = 0xfe;
    auto n = bp128d1::encode( in.data(), bp128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 18u ) );
    expect( hexify( out, n ), equal_to( "01fe00000000010000000000000000000000" ) );
    auto n_dec = bp128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp256d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<bp256d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp256d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = bp256d1::encode( in.data(), bp256d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 387u ) );
    expect( hexify( out, n ),
            equal_to( "4c0282008043e41e61966d2b3167d240f605f687642d3e30e30390bef12818cf18"
                      "5117b837c42945762a43b695ee86100b4397e111b315d903a0c3765c5532d752a2"
                      "a238f6f8327f59365780cc134a22c5627407cfb0092e34537011cc04458a6d9559"
                      "e6576255874371445951636b84d25282153419c65d54990b049974b0c10955d8f3"
                      "775641d84931c3e15c5d95b8491ae79190534577713162761f4b03d26cdfc074f4"
                      "7538573739558628942d8bf123070f514242759a496689b6977f668ad499bd088d"
                      "c63507089f29262eb4d5f6131221080cd5d0213aabfd653afd826875b0b5684a89"
                      "439cd4377578a7f4f8287da8460953c9a9236b2568108911772604b33ca8241544"
                      "27041712d0e576384e9160220855148e84b27a5c552186793e5591500736cf1386"
                      "496b3a933780e7620e999cd2480b6f803ba455f7351a08c32aff748b4cb9b94d42"
                      "e7f026739430376851263f9346b6089f65168a47a0620aa7f461dbb57420eb4612"
                      "49afd9867d9936977e3a04865465d93114a79484046f5989" ) );
    auto n_dec = bp256d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<bp128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp128d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = bp128d1::encode( in.data(), bp128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 195u ) );
    expect( hexify( out, n ),
            equal_to( "4c0282007048381e0133662be11b7340f68c5f642de4e8e9036d05f628d2861051"
                      "f670137be3119964d703615b795c71b1d052c4395b822a033af4ee56250843272a"
                      "291307f6f8c72e59463811ccc3a28ac5d20c2fb309337355704ca1044556469759"
                      "e657412587d345145941903584129c2d36195563579944b896746b01055582881c"
                      "7f9790709d74716bce751f2d9bd46c563155443113237c5db534691af70dbe470f"
                      "f415657537a908892864c866f1a325874342995345667bd9927f4872d099" ) );
    auto n_dec = bp128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp256d1: encode STEPLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<bp256d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp256d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + bp256d1::STEPLEN, bp256d1::BLOCKLEN - bp256d1::STEPLEN,
                 in[bp256d1::STEPLEN - 1] );
    auto n = bp256d1::encode( in.data(), bp256d1::STEPLEN, out.data() );
    expect( n, equal_to( 35u ) );
    expect( hexify( out, n ),
            equal_to( "4c0282000000001e0100002b010000400600008704000030030000be0100"
                      "00cf080000" ) );
    auto n_dec = bp256d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp128d1: encode STEPLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    std::mt19937 mt{ 0x42421337 };
    std::uniform_int_distribution<bp128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp128d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + bp128d1::STEPLEN, bp128d1::BLOCKLEN - bp128d1::STEPLEN,
                 in[bp128d1::STEPLEN - 1] );
    auto n = bp128d1::encode( in.data(), bp128d1::STEPLEN, out.data() );
    expect( n, equal_to( 19u ) );
    expect( hexify( out, n ), equal_to( "4b0282000000001e0100002b01000040060000" ) );
    auto n_dec = bp128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
