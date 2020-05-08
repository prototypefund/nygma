// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/index-resultset.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "index-resultset basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;
  constexpr auto F = detail::resultset_kind::FORWARD;
  constexpr auto R = detail::resultset_kind::REVERSE;
  using resultset32 = resultset<detail::std_vector_traits<std::uint32_t>, F>;
  using resultset64 = riot::resultset<riot::detail::std_vector_traits<std::uint64_t>, F>;

  test( "detail::resultset constructor", []( auto& expect ) {
    resultset32 const rs{ 0 };
    expect( rs.empty() );
  } );

  test( "detail::resultset<reverse> constructor", []( auto& expect ) {
    resultset<detail::std_vector_traits<std::uint32_t>, R> r{ 0 };
    expect( r.empty() );
    expect( ! r );
  } );

  test( "union of empty forward and reverse sets", []( auto& expect ) {
    resultset<detail::std_vector_traits<std::uint32_t>, R> r{ 0 };
    resultset<detail::std_vector_traits<std::uint32_t>, F> f{ 0 };
    auto const x = r + f;
    expect( r.empty() );
    expect( ! r );
    expect( ! f );
    expect( ! x );
  } );

  test( "detail::resultset union of empty sets is empty", []( auto& expect ) {
    resultset32 const a{ 0 };
    resultset32 const b{ 0 };
    auto const c = a + b;
    expect( c.empty() );
  } );

  test( "detail::resultset union with self", []( auto& expect ) {
    resultset32 const a{ 0, 3u };
    auto const c = a + a;
    expect( c.size(), equal_to( 1u ) );
    expect( c.values(), equal_to( { 3u } ) );
  } );

  test( "detail::resultset union of 2 disjoint sets", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 5u, 9u, 10u };
    resultset32 const b{ 0, 2u, 6u, 11u, 12u };
    auto const c = a + b;
    expect( c.size(), equal_to( 8u ) );
    expect( c.values(), equal_to( { 2u, 3u, 5u, 6u, 9u, 10u, 11u, 12u } ) );
  } );

  test( "detail::resultset no duplicates in union of 2 sets", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 6u, 9u, 10u };
    resultset32 const b{ 0, 2u, 6u, 11u, 12u };
    auto const c = a + b;
    expect( c.size(), equal_to( 7u ) );
    expect( c.values(), equal_to( { 2u, 3u, 6u, 9u, 10u, 11u, 12u } ) );
  } );

  test( "detail::resultset intersection of empty sets is empty", []( auto& expect ) {
    resultset32 const a{ 0 };
    resultset32 const b{ 0 };
    auto const c = a & b;
    expect( c.empty() );
  } );

  test( "union of 2 sets with different segment offset is empty", []( auto& expect ) {
    resultset32 const a{ 23u, true, 3u, 6u, 9u, 10u };
    resultset32 const b{ 42u, true, 2u, 6u, 11u, 12u };
    auto const c = a + b;
    expect( ! c );
    expect( c.empty() );
  } );

  test( "detail::resultset intersection of 2 disjoint sets", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 5u, 9u, 10u };
    resultset32 const b{ 0, 2u, 6u, 11u, 12u };
    auto const c = a & b;
    expect( c.empty() );
  } );

  test( "detail::resultset intersection of 2 sets", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 6u, 9u, 12u };
    resultset32 const b{ 0, 2u, 6u, 11u, 12u };
    auto const c = a & b;
    expect( c.size(), equal_to( 2u ) );
    expect( c.values(), equal_to( { 6u, 12u } ) );
  } );

  test( "detail::resultset complement of empty sets is empty", []( auto& expect ) {
    resultset32 const a{ 0 };
    resultset32 const b{ 0 };
    auto const c = a - b;
    expect( c.empty() );
  } );

  test( "detail::resultset complement ( a - b = a iff a & b = 0 )", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 5u, 9u, 10u };
    resultset32 const b{ 0, 2u, 6u, 11u, 12u };
    auto const c = a - b;
    expect( c.values(), equal_to( { 3u, 5u, 9u, 10u } ) );
  } );

  test( "detail::resultset complement of non disjoint sets", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 5u, 9u, 10u };
    resultset32 const b{ 0, 2u, 5u, 8u, 10u };
    auto const c = a - b;
    expect( c.values(), equal_to( { 3u, 9u } ) );
  } );

  test( "set operation on incompatible resultsets return empty set of first", []( auto& expect ) {
    resultset32 const a{ 0, 3u, 5u, 9u, 10u };
    resultset64 const b{ 0, 2u, 5u, 8u, 10u };
    auto const c = a - b;
    expect( c.empty() );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
