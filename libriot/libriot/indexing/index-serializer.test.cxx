// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>
#include <pest/xoshiro.hxx>

#include <libriot/indexing/index-builder.hxx>
#include <libriot/indexing/index-serializer.hxx>
#include <libriot/indexing/index-view.hxx>

#include <cassert>
#include <map>
#include <sstream>

namespace {

template <typename K, typename V>
using map_type = std::map<K, V>;

emptyspace::pest::suite basic( "index-builder basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "index_builder accepts bytestream serializer", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 2u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 68u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "0401041800000004010410000000010208b60bcc0099616501030208000000000700000037133713fef"
                  "e23010e0000001900000000000000000000003713371341414141" ) );
  } );

  test( "index_builder for 32bit keys", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;

    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 91u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to(
            "0401042c01000004020818000000b80b0000040208100000009001000001030c01000000b60bcc00996165010"
            "3030c00000000070000001200000037133713fefe23011d0000002c0000000000000000000000371337134141"
            "4141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );

    auto const iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->is_ex(), equal_to( false ) );
    expect( iv->query( 1 ), equal_to( { 300u } ) );
    expect( iv->query_ex( 1 ).size(), equal_to( 0u ) );
    expect( iv->query( 13372342u ), equal_to( { 24u, 3000u } ) );
    expect( iv->query( 23421337u ), equal_to( { 16u, 400u } ) );
  } );

  test( "index_builder for 16bit keys", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint16_t, map_type, 128>;
    index_type idx;

    idx.add( 65531u, 16 );
    idx.add( 80u, 24 );
    idx.add( 65531u, 400 );
    idx.add( 1337u, 300 );
    idx.add( 80u, 3000 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 85u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "04020818000000b80b00000401042c010000040208100000009001000001030650003905fbff03030c0"
                  "00000000b0000001200000037133713fefe23001d000000260000000000000000000000371337134141"
                  "4141" ) );
  } );

  test( "index_builder for 64bit keys", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint64_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 103u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "0401042c01000004020818000000b80b000004020810000000900100000103180100000000000000b60"
                  "bcc0000000000996165010000000003030c00000000070000001200000037133713fefe23021d000000"
                  "3800000000000000000000003713371341414141" ) );
  } );

  test( "index_builder for 128bit keys", []( auto& expect ) {
    using index_type = riot::index_builder<__uint128_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 127u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "0401042c01000004020818000000b80b000004020810000000900100000103300100000000000000000"
                  "0000000000000b60bcc000000000000000000000000009961650100000000000000000000000003030c"
                  "00000000070000001200000037133713fefe23031d00000050000000000000000000000037133713414"
                  "14141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );

    auto const iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->is_ex(), equal_to( true ) );
    expect( iv->query_ex( 1 ), equal_to( { 300u } ) );
    expect( iv->query( 1 ).size(), equal_to( 0u ) );
    expect( iv->query_ex( 13372342u ), equal_to( { 24u, 3000u } ) );
    expect( iv->query_ex( 23421337u ), equal_to( { 16u, 400u } ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
