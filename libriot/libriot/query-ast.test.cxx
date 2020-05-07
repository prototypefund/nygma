// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/query-ast.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-ast basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "ast-builder: build number literal", []( auto& expect ) {
    auto const node = ast::number( source_span::null(), 0x2343u );
    std::uint64_t n;
    node->accept<kind::NUM>( [&]( auto const& number_node ) { n = number_node._value; } );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 0x2343u ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::ID>( []( auto const& ) {} ); } ) );
  } );

  test( "ast-builder: eval number literal", []( auto& expect ) {
    auto const node = ast::number( source_span::null(), 0x2343u );
    auto const n = node->eval<kind::NUM>(
        [&]( auto const& number_node ) { return number_node._value; } );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 0x2343u ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::ID>( []( auto const& ) {} ); } ) );
  } );

  test( "ast-builder: eval number literal using {overloaded}", []( auto& expect ) {
    auto const node = ast::number( source_span::null(), 0x2343u );
    auto const n = node->eval( riot::overloaded{
        [&]( riot::number const& number_node ) { return number_node._value; },
        []( auto const& ) { return 0ul; } } );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 0x2343u ) );
    expect( throws<riot::expression_coercion_error>(
        [&]() { node->accept<kind::ID>( []( auto const& ) {} ); } ) );
  } );

  test( "ast-builder: mock evalutator", []( auto& expect ) {
    auto const node = ast::number( source_span::null(), 0x2343u );
    struct mock {
      int operator()( ident const& ) const { return 1; }
      int operator()( number const& ) const { return 1; }
      int operator()( ipv4 const& ) const { return 1; }
      int operator()( ipv6 const& ) const { return 1; }
      int operator()( query const& q ) const {
        return q._name->eval( *this ) + q._what->eval( *this );
      }
      int operator()( binary const& b ) const { return b._a->eval( *this ) + b._b->eval( *this ); }
    };
    auto const n = node->eval( mock{} );
    expect( node->type() == kind::NUM );
    expect( n, equal_to( 1 ) );
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
    auto ix = ast::ident( source_span::null(), "ix" );
    auto const query = ast::lookup_forward( source_span::null(), std::move( ix ), std::move( what ) );
    std::string name;
    std::uint32_t ip;
    query_method m;
    query->accept<kind::QUERY>( [&]( auto const& q ) {
      q._name->template accept<kind::ID>( [&]( auto const& id ) { name = id._name; } );
      m = q._method;
      q._what->template accept<kind::IPV4>( [&]( auto const& w ) { ip = w._value; } );
    } );
    expect( query->type() == kind::QUERY );
    expect( m == query_method::FORWARD );
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
