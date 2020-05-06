// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/index-resultset.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "index-resultset basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  using resultset32 = riot::resultset<riot::detail::std_vector_traits<std::uint32_t>>;

  test( "detail::resultset constructor", []( auto& expect ) {
    resultset32 const rs{ 0 };
    expect( rs.empty() );
  } );

  test( "detail::resultset union of empty sets is empty", []( auto& expect ) {
    resultset32 const a{ 0 };
    resultset32 const b{ 0 };
    auto const c = a + b;
    expect( c.empty() );
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
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
