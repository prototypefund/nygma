// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libriot/query-parser.hxx>

#include <string_view>

namespace {

auto hexify_v6( __uint128_t const value ) noexcept {
  std::byte buf[sizeof( __uint128_t )];
  std::memcpy( buf, &value, sizeof( __uint128_t ) );
  return emptyspace::pest::hexify( buf );
};

emptyspace::pest::suite basic( "query-parser basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "parselet::id", []( auto& expect ) {
    std::string_view const query{ " ix " };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::ID>( [&]( auto const& id ) { expect( id._name, equal_to( "ix" ) ); } );
  } );

  test( "ipv4 literal 127.0.0.1", []( auto& expect ) {
    std::string_view const query{ "127.0.0.1" };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::IPV4>( [&]( auto const& ip ) { expect( ip._value, equal_to( 2130706433u ) ); } );
  } );

  test( "ipv6 literal ::1:2:3:4:5", []( auto& expect ) {
    std::string_view const query{ "::1:2:3:4:5" };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::IPV6>( [&]( auto const& ip ) {
      expect( hexify_v6( ip._value ), equal_to( "00000000000000010002000300040005" ) );
    } );
  } );

  test( "ipv6 literal 2606:2800:220:1:248:1893:25c8:1946", []( auto& expect ) {
    std::string_view const query{ "2606:2800:220:1:248:1893:25c8:1946" };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::IPV6>( [&]( auto const& ip ) {
      expect( hexify_v6( ip._value ), equal_to( "26062800022000010248189325c81946" ) );
    } );
  } );

  test( "ipv6 literal 2001:41c0::645:a65e:60ff:feda:589d", []( auto& expect ) {
    std::string_view const query{ "2001:41c0::645:a65e:60ff:feda:589d" };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::IPV6>( [&]( auto const& ip ) {
      expect( hexify_v6( ip._value ), equal_to( "200141c000000645a65e60fffeda589d" ) );
    } );
  } );

  test( "ipv6 literal ::", []( auto& expect ) {
    std::string_view const query{ "::" };
    auto q = riot::parse( query );
    expect( ! ! q );
    q->accept<kind::IPV6>( [&]( auto const& ip ) {
      expect( hexify_v6( ip._value ), equal_to( "00000000000000000000000000000000" ) );
    } );
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

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
