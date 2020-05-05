// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/query-parser.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-parser basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "parselet::id", []( auto& expect ) {
    std::string_view const query{ " ix " };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::ID>( [&]( auto const& id ) { expect( id._name, equal_to( "ix" ) ); } );
  } );

  test( "expression: 'ix( 53 )'", []( auto& expect ) {
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

  test( "expression: '( ix( 53 ) )'", []( auto& expect ) {
    std::string_view const input{ "( ix( 53 ) )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::QUERY>( [&]( auto const& query ) {
      query._name->template accept<kind::ID>(
          [&]( auto const& id ) { expect( id._name, equal_to( "ix" ) ); } );
      query._what->template accept<kind::NUM>(
          [&]( auto const& n ) { expect( n._value, equal_to( 53u ) ); } );
    } );
  } );

  test( "expression: 'ix( 53 ) & ix( 80 )'", []( auto& expect ) {
    std::string_view const input{ "ix( 53 ) & ix( 80 )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::BINARY>( [&]( auto const& binary ) {
      expect( binary._op == binop::INTERSECTION );
      expect( binary._a->type() == kind::QUERY );
      expect( binary._b->type() == kind::QUERY );
    } );
  } );

  test( "expression: 'ix( 53 ) \\ ix( 80 )'", []( auto& expect ) {
    std::string_view const input{ "ix( 53 ) \\ ix( 80 )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::BINARY>( [&]( auto const& binary ) {
      expect( binary._op == binop::COMPLEMENT );
      expect( binary._a->type() == kind::QUERY );
      expect( binary._b->type() == kind::QUERY );
    } );
  } );

  test( "expression: 'ix( 53 ) - ix( 80 )'", []( auto& expect ) {
    std::string_view const input{ "ix( 53 ) - ix( 80 )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::BINARY>( [&]( auto const& binary ) {
      expect( binary._op == binop::COMPLEMENT );
      expect( binary._a->type() == kind::QUERY );
      expect( binary._b->type() == kind::QUERY );
    } );
  } );

  test( "expression: 'ix( 53 ) + ix( 80 )'", []( auto& expect ) {
    std::string_view const input{ "ix( 53 ) + ix( 80 )" };
    auto q = riot::parse( input );
    expect( ! ! q );
    q->accept<kind::BINARY>( [&]( auto const& binary ) {
      expect( binary._op == binop::UNION );
      expect( binary._a->type() == kind::QUERY );
      expect( binary._b->type() == kind::QUERY );
    } );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
