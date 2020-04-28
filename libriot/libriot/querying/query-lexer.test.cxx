// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <pest/pest.hxx>

#include <libriot/querying/query-lexer.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-lexer basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "token constructor & accessors", []( auto& expect ) {
    token tok{ token_type::LB, 0, 1 };
    expect( tok.type(), equal_to( token_type::LB ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );
  
  test( "scan whitespace only", []( auto& expect ) {
    std::string_view data{ "       " };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::EOS ) );
    expect( tok.offset(), equal_to( 7u ) );
    expect( tok.size(), equal_to( 0u ) );
  } );

  test( "scanner constructor & scan LP", []( auto& expect ) {
    std::string_view data{ "(" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::LP ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 1u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan number `0`", []( auto& expect ) {
    std::string_view data{ "0" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 1u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan number `1234`", []( auto& expect ) {
    std::string_view data{ "1234" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 4u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 4u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan & slice number `1234`", []( auto& expect ) {
    std::string_view data{ "1234" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 4u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 4u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "1234" ) );
  } );

  test( "scan & slice ipv4 literal `255.255.255.0`", []( auto& expect ) {
    std::string_view data{ "255.255.255.0" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::IPV4 ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 13u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 13u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "255.255.255.0" ) );
  } );

  test( "scan & slice ipv6 literal `::1`", []( auto& expect ) {
    std::string_view data{ "::1" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::IPV6 ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 3u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 3u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "::1" ) );
  } );

  test( "scan & slice ipv6 literal `0:0:0:0:0:0:0:1`", []( auto& expect ) {
    std::string_view data{ "0:0:0:0:0:0:0:1" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::IPV6 ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 15u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 15u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "0:0:0:0:0:0:0:1" ) );
  } );

  test( "scanner is non validating, e.g. ipv4 = `999.999.999.999.999`", []( auto& expect ) {
    std::string_view data{ "999.999.999.999.999" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::IPV4 ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 19u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 19u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "999.999.999.999.999" ) );
  } );
  
  test( "scanner ID = `i4`", []( auto& expect ) {
    std::string_view data{ "i4" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::ID ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 2u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 2u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "i4" ) );
  } );
} );

}
int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
