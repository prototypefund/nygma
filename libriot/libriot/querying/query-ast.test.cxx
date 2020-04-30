// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <pest/pest.hxx>

#include <libriot/querying/query-ast.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-ast basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "ast-builder: build ipv4 literal", []( auto& expect ) {
    auto const node = ast::number( source_span::null(), 0x2343u );
    std::uint64_t n;
    node->accept<kind::NUM>( [&]( auto const& number_node ) { n = number_node._value; } );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 0x2343u ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::ID>( []( auto const& ) {} ); } ) );
  } );

  test( "ast-builder: build ident", []( auto& expect ) {
    auto const node = ast::ident( source_span::null(), "ix" );
    std::string name;
    node->accept<kind::ID>( [&]( auto const& id ) { name = id._name; } );
    expect( node->type() == kind::ID );
    expect( name, equal_to( "ix" ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::NUM>( []( auto const& ) {} ); } ) );
  } );

  test( "ast-builder: build query", []( auto& expect ) {
    auto what = ast::ipv4( source_span::null(), 0x2342u );
    auto const query = ast::lookup( source_span::null(), "ix", std::move( what ) );
    std::string name;
    std::uint32_t ip;
    query->accept<kind::QUERY>( [&]( auto const& q ) {
      name = q._name;
      q._what->template accept<kind::IPV4>( [&]( auto const& w ) { ip = w._value; } );
    } );
    expect( query->type() == kind::QUERY );
    expect( ip, equal_to( 0x2342u ) );
    expect( name, equal_to( "ix" ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { query->accept<kind::NUM>( []( auto const& ) {} ); } ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
