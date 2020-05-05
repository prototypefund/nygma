// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/query-parser.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-ast basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "parselet::id", []( auto& expect ) {
    std::string_view const query{ " ix " };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::ID>( [&]( auto const& id ) { expect( id._name, equal_to( "ix" ) ); } );
  } );

  test( "lookup forward port 53", []( auto& expect ) {
    std::string_view const input{ "ix( 53 )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::QUERY>( [&]( auto const& query ) {
      query._name->template accept<kind::ID>(
          [&]( auto const& id ) { expect( id._name, equal_to( "ix" ) ); } );
      query._what->template accept<kind::NUM>(
          [&]( auto const& n ) { expect( n._value, equal_to( 53u ) ); } );
    } );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
