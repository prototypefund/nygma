// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libunclassified/format.hxx>

namespace {

emptyspace::pest::suite basic( "format basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace unclassified::format;

  test( "test 2342ull", []( auto& expect ) {
    unsigned long long x = 2342ull;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, x );

    expect( s._offset, equal_to( 4u ) );
    expect( s.view(), equal_to( "2342" ) );
  } );

  test( "test double", []( auto& expect ) {
    double x = .123;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, x );

    expect( s._offset, equal_to( 5u ) );
    expect( s.view(), equal_to( "0.123" ) );
  } );

  test( "test double < 0", []( auto& expect ) {
    double x = -.123;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, x );

    expect( s._offset, equal_to( 6u ) );
    expect( s.view(), equal_to( "-0.123" ) );
  } );

  test( "test", []( auto& expect ) {
    long long x = -1ll;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, x );

    expect( s._offset, equal_to( 2u ) );
    expect( s.view(), equal_to( "-1" ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned long long x = 0x2342ull;
    auto l = ilog16( x );

    expect( l, equal_to( 4u ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned long long x = 0x2342ull;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, hex( x ) );

    expect( s._offset, equal_to( 6u ) );
    expect( s.view(), equal_to( "0x2342" ) );
  } );

  test( "test", []( auto& expect ) {
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, dec( 'A' ) );

    expect( s._offset, equal_to( 2u ) );
    expect( s.view(), equal_to( "65" ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned x = 0x1234;
    char buf[128];

    auto s = stream<128>( buf );
    encode( s, "[ ", "XXX ", hex( x ), 'i' );

    expect( s._offset, equal_to( 13u ) );
    expect( s.view(), equal_to( "[ XXX 0x1234i" ) );
  } );

  test( "test oct(x)", []( auto& expect ) {
    unsigned long long x = 0644ull;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, oct( x ) );

    expect( s._offset, equal_to( 4u ) );
    expect( s.view(), equal_to( "0644" ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned long long x = 0xabcdull;
    char buf[32];

    auto s = stream<32>( buf );
    encode( s, hex( x ) );

    expect( s._offset, equal_to( 6u ) );
    expect( s.view(), equal_to( "0xabcd" ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned long x = 0x12345ul;
    auto l = ilog16( x );

    expect( l, equal_to( 5u ) );
  } );

  test( "test", []( auto& expect ) {
    unsigned x = 0xffffffffu;
    auto l = ilog16( x );

    expect( l, equal_to( 8u ) );
  } );

  test( "test", []( auto& expect ) {
    int x = -1;
    auto l = ilog16( x );

    expect( l, equal_to( 1l ) );
  } );

  test( "timestamp", []( auto& expect ) {
    const auto then = 1546966223006057057ull;
    char buf[TIMESTAMP_BUFSZ];
    auto s = stream<TIMESTAMP_BUFSZ>( buf );
    s.encode( ts( then ) );
    expect( s.view(), equal_to( "2019.01.08-16:50:23.006" ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
