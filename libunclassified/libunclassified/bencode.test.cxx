// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libunclassified/bencode.hxx>

namespace bencode = unclassified::bencode;
using istream = bencode::istream;
using type = bencode::type;
using build = bencode::build;

namespace {

emptyspace::pest::suite basic( "bencode basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  test( "embed double in value", []( auto& expect ) {
    auto v = bencode::value{ 13.37 };
    expect( v.to_int(), equal_to( 13 ) );
    expect( v.to_number(), equal_to( 13.37 ) );
  } );

  test( "parse single integer", []( auto& expect ) {
    auto data = istream{ "i42e" };
    bool error = false;
    bool is_number = false;
    bool not_a_number = false;
    std::int64_t number = 0;
    bencode::parse( data, [&]( auto&& v ) {
      using T = std::decay_t<decltype( v )>;
      if constexpr( std::is_same_v<T, bencode::failed> ) {
        error = true;
      } else if constexpr( std::is_same_v<T, bencode::number> ) {
        is_number = v.is_number();
        number = v.to_int( 23 );
      } else {
        not_a_number = true;
      }
    } );
    expect( not_a_number, equal_to( false ) );
    expect( error, equal_to( false ) );
    expect( is_number, equal_to( true ) );
    expect( number, equal_to( 42 ) );
    expect( data.cursor(), equal_to( 4u ) );
  } );

  test( "parse single integer into stack of values", []( auto& expect ) {
    auto data = istream{ "i42e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 4u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].to_int(), equal_to( 42 ) );
  } );

  test( "parse single negative integer into stack of values", []( auto& expect ) {
    auto data = istream{ "i-42e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 5u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].to_int(), equal_to( -42 ) );
  } );

  test( "parse two integers into stack of values", []( auto& expect ) {
    auto data = istream{ "i42ei23e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 8u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].to_int(), equal_to( 42 ) );
    expect( stack[1].to_int(), equal_to( 23 ) );
  } );

  test( "parse fails with unexpected character at end of data", []( auto& expect ) {
    auto data = istream{ "i42ei23e " };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 8u ) );
    expect( stack.size(), equal_to( 3u ) );
    expect( stack[0].to_int(), equal_to( 42 ) );
    expect( stack[1].to_int(), equal_to( 23 ) );
    expect( stack[2].is_error(), equal_to( true ) );
    expect( stack[2].error_code(), equal_to( bencode::error::unexpected_character ) );
  } );

  test( "parse fails empty integer with invalid number", []( auto& expect ) {
    auto data = istream{ "ie" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 1u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_error(), equal_to( true ) );
    expect( stack[0].error_code(), equal_to( bencode::error::invalid_number ) );
  } );

  test( "parse fails '-' only integer", []( auto& expect ) {
    auto data = istream{ "i-e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 2u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_error(), equal_to( true ) );
    expect( stack[0].error_code(), equal_to( bencode::error::invalid_number ) );
  } );

  test( "parse fails with unexpected character in the middle data", []( auto& expect ) {
    auto data = istream{ "i42e i23e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 4u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].to_int(), equal_to( 42 ) );
    expect( stack[1].is_error(), equal_to( true ) );
    expect( stack[1].error_code(), equal_to( bencode::error::unexpected_character ) );
  } );

  test( "parse empty list", []( auto& expect ) {
    auto data = istream{ "le" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 2u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_list(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 1u ) );
    expect( stack[1].is_end(), equal_to( true ) );
    expect( stack[1].payload(), equal_to( 2u ) );
  } );

  test( "parse empty dict", []( auto& expect ) {
    auto data = istream{ "de" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 2u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_dict(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 1u ) );
    expect( stack[1].is_end(), equal_to( true ) );
    expect( stack[1].payload(), equal_to( 2u ) );
  } );

  test( "parse bytestring with length 1", []( auto& expect ) {
    auto data = istream{ "1:a" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      if( v.is_bytestring() ) {
        stack.push_back( v );
        string = data.slice( v.payload() );
      }
    } );
    expect( data.cursor(), equal_to( 3u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 1u ) );
    expect( string, equal_to( "a" ) );
  } );

  test( "parse bytestring with length 0", []( auto& expect ) {
    auto data = istream{ "0:" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      if( v.is_bytestring() ) {
        stack.push_back( v );
        string = data.slice( v.payload() );
      }
    } );
    expect( data.cursor(), equal_to( 2u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 0u ) );
    expect( string, equal_to( "" ) );
  } );

  test( "parse bytestring fails with negative length", []( auto& expect ) {
    auto data = istream{ "-1:a" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      stack.push_back( v );
      if( v.is_bytestring() ) { string = data.slice( v.payload() ); }
    } );
    expect( data.cursor(), equal_to( 0u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_error(), equal_to( true ) );
    expect( stack[0].error_code(), equal_to( bencode::error::unexpected_character ) );
  } );

  test( "parse bytestring does not read beyond end 1", []( auto& expect ) {
    auto data = istream{ "2:a" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 1u ) );
    expect( stack.size(), equal_to( 1u ) );
    expect( stack[0].is_error(), equal_to( true ) );
    expect( stack[0].error_code(), equal_to( bencode::error::invalid_bytestring_length ) );
  } );

  test( "parse bytestring does not interprete contents if user does not slice", []( auto& expect ) {
    auto data = istream{ "4:i42e" };
    std::vector<bencode::value> stack;
    bencode::parse( data, [&]( bencode::value v ) { stack.push_back( v ); } );
    expect( data.cursor(), equal_to( 1u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 4u ) );
    expect( stack[1].is_error(), equal_to( true ) );
    expect( stack[1].error_code(), equal_to( bencode::error::unexpected_character ) );
  } );

  test( "parse bytestring does not interpret bytestring", []( auto& expect ) {
    auto data = istream{ "2:aii10e" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      stack.push_back( v );
      if( v.is_bytestring() ) { string = data.slice( v.payload() ); }
    } );
    expect( data.cursor(), equal_to( 8u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 2u ) );
    expect( stack[1].is_number(), equal_to( true ) );
    expect( stack[1].to_int(), equal_to( 10 ) );
    expect( string, equal_to( "ai" ) );
  } );

  test( "parse bytestring fails in the middle of next value", []( auto& expect ) {
    auto data = istream{ "3:aii10e" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      stack.push_back( v );
      if( v.is_bytestring() ) { string = data.slice( v.payload() ); }
    } );
    expect( data.cursor(), equal_to( 7u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 3u ) );
    expect( stack[1].is_error(), equal_to( true ) );
    expect( stack[1].error_code(), equal_to( bencode::error::invalid_bytestring_length ) );
    expect( string, equal_to( "aii" ) );
  } );

  test( "parse bytestring fails in the middle of next valuei 2", []( auto& expect ) {
    auto data = istream{ "3:aii10e0123456789" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      stack.push_back( v );
      if( v.is_bytestring() ) { string = data.slice( v.payload() ); }
    } );
    expect( data.cursor(), equal_to( 7u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 3u ) );
    expect( stack[1].is_error(), equal_to( true ) );
    expect( stack[1].error_code(), equal_to( bencode::error::invalid_bytestring ) );
    expect( string, equal_to( "aii" ) );
  } );

  test( "parse two bytestrings check value of last", []( auto& expect ) {
    auto data = istream{ "3:aii10:0123456789" };
    std::vector<bencode::value> stack;
    auto string = std::string_view{};
    bencode::parse( data, [&]( bencode::value v ) {
      stack.push_back( v );
      if( v.is_bytestring() ) { string = data.slice( v.payload() ); }
    } );
    expect( data.cursor(), equal_to( 18u ) );
    expect( stack.size(), equal_to( 2u ) );
    expect( stack[0].is_bytestring(), equal_to( true ) );
    expect( stack[0].payload(), equal_to( 3u ) );
    expect( stack[1].is_bytestring(), equal_to( true ) );
    expect( stack[1].payload(), equal_to( 10u ) );
    expect( string, equal_to( "0123456789" ) );
  } );

  test( "expect that number is 42", []( auto& expect ) {
    auto data = istream{ "i42e" };
    std::int64_t number = 0;
    bencode::expect<type::number>( data, [&]( bencode::value v ) { number = v.to_int( 23 ); } );
    expect( data.cursor(), equal_to( 4u ) );
    expect( number, equal_to( 42 ) );
  } );

  test( "expect bytestring is not a number", []( auto& expect ) {
    auto data = istream{ "1:a" };
    std::int64_t number = 0;
    bencode::expect<type::number>( data, [&]( bencode::value v ) { number = v.to_int( 23 ); } );
    expect( data.cursor(), equal_to( 0u ) );
    expect( number, equal_to( 23 ) );
  } );

  test( "monadic expect bytestring", []( auto& expect ) {
    auto data = bencode::dstream{ "1:a" };
    std::string_view s;
    data.then<type::bytestring>( [&]( auto const v ) { s = data.slice( v.payload() ); } );
    expect( data._failed, equal_to( false ) );
    expect( data.cursor(), equal_to( 3u ) );
    expect( s, equal_to( "a" ) );
  } );

  test( "can build positive number", []( auto& expect ) {
    char buf[512];
    auto build = bencode::build( buf );

    build << 42;

    expect( build.success(), equal_to( true ) );
    expect( build.used(), equal_to( 4u ) );
    expect( std::string_view( buf, build.used() ), equal_to( "i42e" ) );
  } );

  test( "can build negative number", []( auto& expect ) {
    char buf[512];
    auto build = bencode::build( buf );

    build << -42;

    expect( build.success(), equal_to( true ) );
    expect( build.used(), equal_to( 5u ) );
    expect( std::string_view( buf, build.used() ), equal_to( "i-42e" ) );
  } );

  test( "can build empty list", []( auto& expect ) {
    char buf[512];
    auto build = bencode::build( buf );

    build << build::list << build::end;

    expect( build.success(), equal_to( true ) );
    expect( build.used(), equal_to( 2u ) );
    expect( std::string_view( buf, build.used() ), equal_to( "le" ) );
  } );

  test( "can build empty dict", []( auto& expect ) {
    char buf[512];
    auto build = bencode::build( buf );

    build << build::dict << build::end;

    expect( build.success(), equal_to( true ) );
    expect( build.used(), equal_to( 2u ) );
    expect( std::string_view( buf, build.used() ), equal_to( "de" ) );
  } );

  test( "can build bytestring", []( auto& expect ) {
    char buf[512];
    auto build = bencode::build( buf );

    build << -42;

    expect( build.success(), equal_to( true ) );
    expect( build.used(), equal_to( 5u ) );
    expect( std::string_view( buf, build.used() ), equal_to( "i-42e" ) );
  } );
} );
}
int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
