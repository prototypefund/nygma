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
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
