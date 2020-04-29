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
    auto node = ast::number( source_span::null(), 0x2343u );
    std::uint64_t n;
    node->accept<kind::NUM>( [&]( auto const number_node ) { n = number_node._value; } );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 0x2343u ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::ID>( []( auto const ) {} ); } ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
