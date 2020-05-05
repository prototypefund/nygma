// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/compress-vbyte.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <bitset>
#include <tuple>
#include <vector>

namespace {

inline std::string bitify( std::byte x ) {
  std::bitset<8> bits{ std::uint8_t( x ) };
  return ( bits.to_string() );
}

emptyspace::pest::suite basic( "vbyte & vbkey compression suite", []( auto& test ) {
  using namespace emptyspace::pest;
  namespace vbkey = riot::vbkey;
  namespace vbyte = riot::vbyte;

  test( "expected encoded length of integers", []( auto& expect ) {
    expect( vbkey::nencode( 0u ), equal_to( 1u ) );
    expect( vbkey::nencode( 1ull << 6 ), equal_to( 1u ) );
    expect( vbkey::nencode( 1ull << 7 ), equal_to( 2u ) );
    expect( vbkey::nencode( 1ull << 8 ), equal_to( 2u ) );
    expect( vbkey::nencode( 1ull << 13 ), equal_to( 2u ) );
    expect( vbkey::nencode( 1ull << 14 ), equal_to( 3u ) );
    expect( vbkey::nencode( 1ull << 16 ), equal_to( 3u ) );
    expect( vbkey::nencode( 1ull << 20 ), equal_to( 3u ) );
    expect( vbkey::nencode( 1ull << 21 ), equal_to( 4u ) );
    expect( vbkey::nencode( 1ull << 24 ), equal_to( 4u ) );
    expect( vbkey::nencode( 1ull << 27 ), equal_to( 4u ) );
    expect( vbkey::nencode( 1ull << 28 ), equal_to( 5u ) );
    expect( vbkey::nencode( 1ull << 32 ), equal_to( 5u ) );
    expect( vbkey::nencode( 1ull << 34 ), equal_to( 5u ) );
    expect( vbkey::nencode( 1ull << 35 ), equal_to( 6u ) );
    expect( vbkey::nencode( 1ull << 40 ), equal_to( 6u ) );
    expect( vbkey::nencode( 1ull << 41 ), equal_to( 6u ) );
    expect( vbkey::nencode( 1ull << 42 ), equal_to( 7u ) );
    expect( vbkey::nencode( 1ull << 48 ), equal_to( 7u ) );
    expect( vbkey::nencode( 1ull << 49 ), equal_to( 8u ) );
    expect( vbkey::nencode( 1ull << 55 ), equal_to( 8u ) );
    expect( vbkey::nencode( 1ull << 56 ), equal_to( 9u ) );
    expect( vbkey::nencode( 1ull << 63 ), equal_to( 9u ) );
  } );

  test( "vbkey: expected encoding of integers", []( auto& expect ) {
    std::byte buf[16];
    std::vector<std::tuple<std::uint64_t, unsigned, std::string, std::string>> values{
        { 1ull << 6, 1, "40", "01000000" },
        { 1ull << 7, 2, "8080", "10000000" },
        { 1ull << 13, 2, "a000", "10100000" },
        { 1ull << 14, 3, "c04000", "11000000" },
        { 1ull << 21, 4, "e0200000", "11100000" },
        { 1ull << 28, 5, "f010000000", "11110000" },
        { 1ull << 35, 6, "f80800000000", "11111000" },
        { 1ull << 42, 7, "fc040000000000", "11111100" },
        { 1ull << 49, 8, "fe02000000000000", "11111110" },
        { 1ull << 56, 9, "ff0100000000000000", "11111111" },
        { ( 1ull << 7 ) - 1, 1, "7f", "01111111" },
        { ( 1ull << 14 ) - 1, 2, "bfff", "10111111" },
        { ( 1ull << 21 ) - 1, 3, "dfffff", "11011111" },
        { ( 1ull << 28 ) - 1, 4, "efffffff", "11101111" },
        { ( 1ull << 35 ) - 1, 5, "f7ffffffff", "11110111" },
        { ( 1ull << 42 ) - 1, 6, "fbffffffffff", "11111011" },
        { ( 1ull << 49 ) - 1, 7, "fdffffffffffff", "11111101" },
        { ( 1ull << 56 ) - 1, 8, "feffffffffffffff", "11111110" },
        { 0xffff'ffff'ffff'ffffull, 9, "ffffffffffffffffff", "11111111" },
    };

    for( auto [v, n, hex, tag] : values ) {
      auto m = vbkey::nencode( v );
      vbkey::encode( buf, m, v );
      auto l = vbkey::ndecode( buf );
      expect( m, equal_to( n ) );
      expect( l, equal_to( n ) );
      expect( hexify( buf, n ), equal_to( hex ) );
      expect( bitify( buf[0] ), equal_to( tag ) );
      auto vv = vbkey::decode( buf, l );
      expect( vv, equal_to( v ) );
    }
  } );

  test( "vbkey: encode timestamp ns", []( auto& expect ) {
    std::byte buf[32];
    // 2019-01-08 16:50:23.006057557
    const auto ts = 1546966223006057057ull;

    auto m = vbkey::nencode( ts );
    vbkey::encode( buf, m, ts );
    auto v = vbkey::decode( buf, m );
    expect( m, equal_to( 9u ) );
    expect( v, equal_to( ts ) );
  } );

  test( "vbkey: encode timestamp us", []( auto& expect ) {
    std::byte buf[32];
    // 2019-01-08 16:50:23.006057557
    const auto ts = 1546966223006057057ull / 1'000;

    auto m = vbkey::nencode( ts );
    vbkey::encode( buf, m, ts );
    auto v = vbkey::decode( buf, m );
    expect( m, equal_to( 8u ) );
    expect( v, equal_to( ts ) );
  } );

  test( "vbkey: encode timestamp ms", []( auto& expect ) {
    std::byte buf[32];
    // 2019-01-08 16:50:23.006057557
    const auto ts = 1546966223006057057ull / 1'000'000;

    auto m = vbkey::nencode( ts );
    vbkey::encode( buf, m, ts );
    auto v = vbkey::decode( buf, m );
    expect( m, equal_to( 6u ) );
    expect( v, equal_to( ts ) );
  } );

  test( "vbkey: encode timestamp delta ms", []( auto& expect ) {
    std::byte buf[32];
    // 2019-01-08 16:50:23.006057557
    auto const ts1 = 1546966223006057057ull / 1'000'000;
    auto const ts2 = 1581514936449402000ull / 1'000'000;
    auto const delta = ts2 - ts1;
    auto const l = vbkey::nencode( ts1 );
    auto const n = vbkey::nencode( ts2 );
    auto const m = vbkey::nencode( delta );
    vbkey::encode( buf, m, delta );
    auto v = vbkey::decode( buf, m );
    expect( l, equal_to( 6u ) );
    expect( n, equal_to( 6u ) );
    expect( m, equal_to( 6u ) );
    expect( v, equal_to( delta ) );
  } );

  test( "vbyte: encode/decode 0", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0 );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b00u ) );
    expect( len, equal_to( 1u ) );
    expect( y, equal_to( 0u ) );
  } );

  test( "vbyte: encode/decode 0xff", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0xffu );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b00u ) );
    expect( len, equal_to( 1u ) );
    expect( y, equal_to( 0xffu ) );
  } );

  test( "vbyte: encode/decode 0x100", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0x100u );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b01u ) );
    expect( len, equal_to( 2u ) );
    expect( y, equal_to( 0x100u ) );
  } );

  test( "vbyte: encode/decode 0x10000", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0x10000u );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b10u ) );
    expect( len, equal_to( 3u ) );
    expect( y, equal_to( 0x10000u ) );
  } );

  test( "vbyte: encode/decode 0x1000000", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0x1000000u );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b11u ) );
    expect( len, equal_to( 4u ) );
    expect( y, equal_to( 0x1000000u ) );
  } );

  test( "vbyte: encode/decode 0xffffffff", []( auto& expect ) {
    std::byte buf[32];
    auto [tag, len] = vbyte::encode( buf, 0xffffffffu );
    auto const y = vbyte::decode( buf, tag );
    expect( tag, equal_to( 0b11u ) );
    expect( len, equal_to( 4u ) );
    expect( y, equal_to( 0xffffffffu ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
