// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>
#include <pest/xoshiro.hxx>
#include <pest/bitmask-distribution.hxx>

#include <libriot/compress-bitpack-simd.hxx>

#include <array>

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
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    emptyspace::bitmask_distribution<bp256d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp256d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = bp256d1::encode( in.data(), bp256d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 387u ) );
    expect( hexify( out, n ),
            equal_to( "4c062b00a02fe3ae328ee6bcb611cde0034c7fa451241e62d01f6f64532c7494003fbb919ac1a15"
                      "414fb3882130d65981bb0b4a50629251345887675358ba1b721dde20e66372332064985700f038d"
                      "13a831802924d205610c22041dda864648c3558f23a4942d2469268b12b7667b6575709f6aa789b"
                      "f415478a3868769901af95d94365cdf37f6790d12d0647b19a606f2c12238db31f0309851f981bc"
                      "e8676d83540fde404f68dde02f5f4a7353057617890463b12a5880a7413fc185984f12e542d4b73"
                      "fc47d2497febb4221a9d97643d848b996fe5d26869f8dc81f1efc952de0893fd522d826a126f241"
                      "7db4274f2c5778185ce0c42759e8565e95798151a1e25b32a42972db559492804543109e261e84d"
                      "d282917d0b3465e689541663d64813ad61d4755c52678e7c3829174a96dcab6e41e288b95945d6c"
                      "022791c0803263a2768c860644a3b9c8057cf2045a45498141fa27c7935564925b36d6241785a88"
                      "db95165221299944041128b8191e72d1bddb09a2022f9267ed4b75085e65681" ) );
    auto n_dec = bp256d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp128d1: check packing delta range <60, 2500>", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    emptyspace::bitmask_distribution<bp128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp128d1::BLOCKLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    auto n = bp128d1::encode( in.data(), bp128d1::BLOCKLEN, out.data() );
    expect( n, equal_to( 195u ) );
    expect( hexify( out, n ),
            equal_to( "4c062b00401afaae2206e3bc46361be04309c05224e3e1d81fe6f4512ccd42043f7fb851a9a9063"
                      "6451145573871357bb9b121c19152eafb8868230db51806b0d42d3b20296647736106c9a71d0fa3"
                      "e148a8312233d205205428046dd088465c03588f23a4764a241944661267786b65a59107a9892df"
                      "652788b3686697b00f75d9f462bc325380a9ff730994df681796a606ddfb71df30d8219817bc98b"
                      "f1f231485c3004de305658dd00e53f4a13504f762716f4b42a78fea2415c3785985191e842" ) );
    auto n_dec = bp128d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp256d1: encode STEPLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> in;
    std::array<std::byte, bp256d1::estimate_compressed_size()> out;
    std::array<bp256d1::integer_type, bp256d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    emptyspace::bitmask_distribution<bp256d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp256d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + bp256d1::STEPLEN, bp256d1::BLOCKLEN - bp256d1::STEPLEN,
                 in[bp256d1::STEPLEN - 1] );
    auto n = bp256d1::encode( in.data(), bp256d1::STEPLEN, out.data() );
    expect( n, equal_to( 35u ) );
    expect( hexify( out, n ),
            equal_to( "4b062b00000000ae020000bc060000e0030000a4010000620000006403000094000000" ) );
    auto n_dec = bp256d1::decode( out.data(), n, dec.data() );
    expect( n_dec, equal_to( n ) );
    expect( in == dec, equal_to( true ) );
  } );

  test( "bp128d1: encode STEPLEN delta range <60, 2500>", []( auto& expect ) {
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> in;
    std::array<std::byte, bp128d1::estimate_compressed_size()> out;
    std::array<bp128d1::integer_type, bp128d1::BLOCKLEN> dec;
    dec.fill( 0xffffffffu );
    emptyspace::xoshiro::xoshiro128starstar32 mt{ 0x42421337 };
    emptyspace::bitmask_distribution<bp128d1::integer_type> random{ 60, 2500 };
    in[0] = random( mt );
    for( std::size_t i = 1; i < bp128d1::STEPLEN; ++i ) { in[i] = in[i - 1] + random( mt ); }
    std::fill_n( in.data() + bp128d1::STEPLEN, bp128d1::BLOCKLEN - bp128d1::STEPLEN,
                 in[bp128d1::STEPLEN - 1] );
    auto n = bp128d1::encode( in.data(), bp128d1::STEPLEN, out.data() );
    expect( n, equal_to( 19u ) );
    expect( hexify( out, n ), equal_to( "4b062b00000000ae020000bc060000e0030000" ) );
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
