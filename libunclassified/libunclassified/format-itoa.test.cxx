// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>
#include <pest/xoshiro.hxx>

#include <libunclassified/format-itoa.hxx>

#include <random>
#include <sstream>

namespace {

emptyspace::pest::suite basic( "format itoa suite", []( auto& test ) {
  namespace xoshiro = emptyspace::xoshiro;
  using namespace emptyspace::pest;
  using namespace unclassified::format;
  namespace detail = unclassified::format::detail;

  test( "itoa(12345)", []( auto& expect ) {
    char buf[32];
    unsigned long long x = 12345ull;
    auto l = detail::ilog10u64( x );
    itoa( buf, l, x );
    auto s = std::string_view{ buf, l };
    expect( l, equal_to( 5u ) );
    expect( s, equal_to( "12345" ) );
  } );

  test( "itoa(12345678901234567890llu)", []( auto& expect ) {
    char buf[32];
    unsigned long long x = 12345678901234567890ull;
    auto l = detail::ilog10u64( x );
    itoa( buf, l, x );
    auto s = std::string_view{ buf, l };
    expect( l, equal_to( 20u ) );
    expect( s, equal_to( "12345678901234567890" ) );
  } );

  test( "itoa(min)", []( auto& expect ) {
    char buf[32];
    unsigned long long x = std::numeric_limits<unsigned long long>::min();
    auto l = detail::ilog10u64( x );
    itoa( buf, l, x );
    auto s = std::string_view{ buf, l };
    expect( l, equal_to( 1u ) );
    expect( s, equal_to( "0" ) );
  } );

  test( "itoa(max)", []( auto& expect ) {
    char buf[32];
    unsigned long long x = std::numeric_limits<unsigned long long>::max();
    auto l = detail::ilog10u64( x );
    itoa( buf, l, x );
    auto s = std::string_view{ buf, l };
    expect( l, equal_to( 20u ) );
    expect( s, equal_to( "18446744073709551615" ) );
  } );

  test( "itoa(12345678901234567890llu)", []( auto& expect ) {
    char buf[32];
    unsigned long long x = 12345678901234567890ull;
    auto l = detail::ilog10u64( x );
    itoa( buf, l, x );
    auto s = std::string_view{ buf, l };
    expect( l, equal_to( 20l ) );
    expect( s, equal_to( "12345678901234567890" ) );
  } );

  test( "bisim sstream/iota mt19937_64", []( auto& expect ) {
    std::mt19937_64 mt{ 0x2342 };
    char buf[32];
    for( int i = 0; i < 10'000; i++ ) {
      unsigned long long x = mt();
      std::ostringstream os;
      os << x;
      auto l = detail::ilog10u64( x );
      itoa( buf, l, x );
      auto s = std::string_view{ buf, l };
      expect( l, equal_to( os.tellp() ) );
      expect( os.str(), equal_to( s ) );
    }
  } );

  test( "bisim sstream/iota xoshiro", []( auto& expect ) {
    xoshiro::xoshiro256starstar64 xo{ 0x2342 };
    char buf[32];
    for( int i = 0; i < 10'000; i++ ) {
      unsigned long long x = xo();
      std::ostringstream os;
      os << x;
      auto l = detail::ilog10u64( x );
      itoa( buf, l, x );
      auto s = std::string_view{ buf, l };
      expect( l, equal_to( os.tellp() ) );
      expect( os.str(), equal_to( s ) );
    }
  } );
} );

}

int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
