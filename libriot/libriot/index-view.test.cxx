// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libriot/index-builder.hxx>
#include <libriot/index-resultset.hxx>
#include <libriot/index-serializer.hxx>
#include <libriot/index-view.hxx>

#include <cassert>
#include <map>
#include <sstream>

namespace {

template <typename K, typename V>
using map_type = std::map<K, V>;

emptyspace::pest::suite basic( "index-view basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "index-view for 32bit keys", []( auto& expect ) {
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

    auto iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 4u ) );
    expect( iv->lookup_forward_32( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_32( 13372342u ).values(), equal_to( { 24u, 3000u } ) );
    expect( iv->lookup_forward_32( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( iv->lookup_reverse( 16 ).values().empty() );
    iv->prepare_reverse_lookups();
    expect( iv->lookup_reverse( 16 ).values(), equal_to( { 16, 400 } ) );
    expect( iv->scan_and( iv->lookup_forward_32( 23421337u ) ).values(), equal_to( { 16, 400 } ) );
    expect( not iv->lookup_forward_128( 1 ) );
  } );

  test( "index-view and reverse lookup for 32bit keys", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;

    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 16 );
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
        equal_to( "0401042c01000004020810000000b80b0000040208100000009001000001030c01000000b60bcc00996"
                  "1650103030c00000000070000001200000037133713fefe23011d0000002c0000000000000000000000"
                  "3713371341414141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );

    auto iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 4u ) );
    expect( iv->lookup_forward_32( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_32( 13372342u ).values(), equal_to( { 16u, 3000u } ) );
    expect( iv->lookup_forward_32( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( iv->lookup_reverse( 16 ).values().empty() );
    iv->prepare_reverse_lookups();
    expect( iv->lookup_reverse( 16 ).values(), equal_to( { 16 } ) );
    expect( iv->scan_and( iv->lookup_forward_32( 23421337u ) ).values(), equal_to( { 16 } ) );
    expect( not iv->lookup_forward_128( 1 ) );

    auto sr = iv->sparse_scan( iv->lookup_forward_32( 23421337u ) );
    expect( sr._values[16]._values, equal_to( { 16u, 400u, 3000u } ) );
  } );

  test( "sparse-scan #1", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;

    index_type idx;
    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 16 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 400 );
    idx.add( 1u, 500 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto os = nygma::cfile_ostream{ data };
    riot::uc128_serializer ser{ os };
    idx.accept( ser, 0u );

    auto const len = static_cast<std::size_t>( os.current_position() );
    auto const iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );
    auto sr = iv->sparse_scan( iv->lookup_forward_32( 23421337u ) );
    expect( sr._values[16]._values, equal_to( { 16u, 400u, 3000u } ) );
    expect( sr._values[400]._values, equal_to( { 16u, 400u, 500u } ) );
  } );

  test( "index-view for 128bit keys", []( auto& expect ) {
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
    idx.accept( ser, 0x41414141u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 127u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to(
            "0401042c01000004020818000000b80b000004020810000000900100000103300100000000000000000000000"
            "0000000b60bcc000000000000000000000000009961650100000000000000000000000003030c000000000700"
            "00001200000037133713fefe23031d0000005000000041414141000000003713371341414141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );

    auto const iv = riot::make_poly_index_view( unclassified::bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 16u ) );
    expect( iv->lookup_forward_128( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_128( 1 ).segment_offset(), equal_to( 0x41414141ull ) );
    expect( iv->lookup_forward_128( 13372342u ).values(), equal_to( { 24u, 3000u } ) );
    expect( iv->lookup_forward_128( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( not iv->lookup_forward_32( 1 ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
