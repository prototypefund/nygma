// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libriot/index-compressor.hxx>
#include <libriot/index-view.hxx>
#include <libunclassified/bytestring.hxx>

#include <cassert>
#include <map>
#include <sstream>

namespace {

using bytestring_view = unclassified::bytestring_view;

template <typename K, typename V>
using map_type = std::map<K, V>;

emptyspace::pest::suite basic( "index-compressor basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "svb128d1 compressed example values", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[2048];
    auto os = nygma::cfile_ostream{ data };
    auto cs = riot::svb128d1_serializer{ os };
    idx.accept( cs, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 104u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "04010b01002c010000000000000004020b040018a00b00000000000004020b040010800100000000000"
                  "001030e280001b50bcce35599000000000003030a0000000e0e000000000037133713020223012a0000"
                  "003b00000000000000000000003713371341414141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );
    auto const iv = riot::make_poly_index_view( bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 4u ) );
    expect( iv->lookup_forward_32( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_32( 13372342u ).values(), equal_to( { 24u, 3000u } ) );
    expect( iv->lookup_forward_32( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( not iv->lookup_forward_128( 1 ) );
  } );

  test( "bp256d1 compressed example values", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 256>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[2048];
    auto os = nygma::cfile_ostream{ data };
    auto cs = riot::bp256d1_serializer{ os };
    idx.accept( cs, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 186u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "04010340012c0402220c1800000000a00b0000000000000000000000000000000000000000000000000"
                  "00004022209100000000080010000000000000000000000000000000000000000000000000000010322"
                  "180100000000b50bcc00e35599000000000000000000000000000000000000000000030322060000000"
                  "00006000000250000000000000000000000000000000000000000000000371337130101230150000000"
                  "7500000000000000000000003713371341414141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );
    auto const iv = riot::make_poly_index_view( bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 4u ) );
    expect( iv->lookup_forward_32( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_32( 13372342u ).values(), equal_to( { 24u, 3000u } ) );
    expect( iv->lookup_forward_32( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( not iv->lookup_forward_128( 1 ) );
  } );

  test( "bp128d1 compressed example values", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[2048];
    auto os = nygma::cfile_ostream{ data };
    auto cs = riot::bp128d1_serializer{ os };
    idx.accept( cs, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.ok(), equal_to( true ) );
    expect( os.current_position(), equal_to( 122u ) );
    expect(
        hexify( data, static_cast<std::size_t>( os.current_position() ) ),
        equal_to( "04010340012c0402120c1800000000a00b0000000000000000000004021209100000000080010000000"
                  "0000000000000010312180100000000b50bcc00e3559900000000000303120500000000000600000015"
                  "000000000000003713371300002301300000004500000000000000000000003713371341414141" ) );

    auto const len = static_cast<std::size_t>( os.current_position() );
    auto const iv = riot::make_poly_index_view( bytestring_view{ data, len } );

    expect( iv->size(), equal_to( 3u ) );
    expect( iv->sizeof_domain_value(), equal_to( 4u ) );
    expect( iv->lookup_forward_32( 1 ).values(), equal_to( { 300u } ) );
    expect( iv->lookup_forward_32( 13372342u ).values(), equal_to( { 24u, 3000u } ) );
    expect( iv->lookup_forward_32( 23421337u ).values(), equal_to( { 16u, 400u } ) );
    expect( not iv->lookup_forward_128( 1 ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
