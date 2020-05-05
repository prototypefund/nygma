// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/index-cycler.hxx>
#include <libriot/index-serializer.hxx>

#include <map>

namespace {

template <typename K, typename V>
using map_type = std::map<K, V>;

emptyspace::pest::suite basic( "index-cycler suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "can instantiate an index-cycler", []( auto& expect ) {
    riot::index_cycler cyc( "/tmp", "index", ".i4" );
    expect( cyc.count(), equal_to( 0u ) );
  } );

  test( "check initial path name of index-cycler", []( auto& expect ) {
    riot::index_cycler cyc( "/tmp", "index", ".i4" );
    expect( cyc.count(), equal_to( 0u ) );
    expect( cyc.path(), equal_to( "/tmp/index-0000.i4" ) );
  } );

  test( "the index-cycler accepts an index-builder", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    std::filesystem::path p;
    {
      // `w`s destructor blocks until all messages got handled
      auto w = std::make_shared<riot::index_writer>();
      auto idx = std::make_unique<index_type>();

      idx->add( 23421337u, 16 );
      idx->add( 13372342u, 24 );
      idx->add( 23421337u, 400 );
      idx->add( 1u, 300 );
      idx->add( 13372342u, 3000 );

      riot::index_cycler cyc( w, "/tmp", "index", ".i4" );
      expect( cyc.count(), equal_to( 0u ) );
      expect( cyc.path(), equal_to( "/tmp/index-0000.i4" ) );
      p = cyc.path();

      cyc.accept<riot::uc128_serializer>( std::move( idx ), 0u );
    }
    std::error_code ec;
    expect( std::filesystem::exists( p, ec ), equal_to( true ) );
    if( not ec ) { std::filesystem::remove( p, ec ); }
    expect( not ec, equal_to( true ) );
  } );

  test( "can instantiate an index-writer", []( auto& expect ) {
    riot::index_writer w;
    w.send( []() {} );
    expect( true, equal_to( true ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
