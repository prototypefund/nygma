// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libriot/index-builder.hxx>
#include <libriot/query-evaluator.hxx>
#include <libriot/query-parser.hxx>

#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace {

template <typename K, typename V>
using map_type = std::map<K, V>;

using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
using bytestring_view = unclassified::bytestring_view;

template <std::size_t N>
std::size_t serialize( std::byte ( &data )[N], index_type& idx ) {
  auto os = nygma::cfile_ostream{ data };
  riot::uc128_serializer ser{ os };
  idx.accept( ser, 0x41414141u );
  return static_cast<std::size_t>( os.current_position() );
}

emptyspace::pest::suite basic( "query-evaluator basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace riot;

  test( "expression: 'ix( 53 ) + ix( 80 )'", []( auto& expect ) {
    std::string_view const input{ "ix( 53 ) + ix( 80 )" };
    auto q = riot::parse( input );
    struct mock {
      int operator()( ident const& ) const { return 1; }
      int operator()( number const& ) const { return 1; }
      int operator()( ipv4 const& ) const { return 1; }
      int operator()( ipv6 const& ) const { return 1; }
      int operator()( query const& q ) const {
        return 1 + q._name->eval( *this ) + q._what->eval( *this );
      }
      int operator()( binary const& b ) const { return 1 + b._a->eval( *this ) + b._b->eval( *this ); }
    };
    auto const n = q->eval( mock{} );
    expect( ! ! q );
    expect( n, equal_to( 7 ) );
  } );

  test( "evaluate: 'ix( 1 )'", []( auto& expect ) {
    index_type idx;
    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto const len = serialize( data, idx );
    auto const env = environment::builder{}.add( "ix", bytestring_view{ data, len } ).build();

    std::string_view const input{ "ix( 1 )" };
    auto const q = riot::parse( input );
    auto const rs = q->eval( env );

    expect( ! ! rs );
    expect( rs.segment_offset(), equal_to( 0x41414141ull ) );
    expect( rs.values(), equal_to( { 300u } ) );
  } );

  test( "evaluate: 'ix( 1 ) + ix( 13372342 )'", []( auto& expect ) {
    index_type idx;
    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::byte data[1024];
    auto const len = serialize( data, idx );
    auto const env = environment::builder{}.add( "ix", bytestring_view{ data, len } ).build();

    std::string_view const input{ "ix( 1 ) + ix( 13372342 )" };
    auto const query = riot::parse( input );
    auto const rs = query->eval( env );

    expect( ! ! rs );
    expect( rs.segment_offset(), equal_to( 0x41414141ull ) );
    expect( rs.values(), equal_to( { 24u, 300u, 3000u } ) );
  } );

  test( "evaluate: 'ix( 1 ) - ix( 2 )'", []( auto& expect ) {
    index_type idx;
    idx.add( 1u, 16 );
    idx.add( 2u, 16 );
    idx.add( 2u, 400 );
    idx.add( 1u, 300 );
    idx.add( 3u, 3000 );

    std::byte data[1024];
    auto const len = serialize( data, idx );
    auto const env = environment::builder{}.add( "ix", bytestring_view{ data, len } ).build();

    std::string_view const input{ "ix( 1 ) - ix( 2 )" };
    auto const query = riot::parse( input );
    auto const rs = query->eval( env );

    expect( ! ! rs );
    expect( rs.segment_offset(), equal_to( 0x41414141ull ) );
    expect( rs.values(), equal_to( { 300u } ) );
  } );

  test( "evaluate: 'ix( 1 ) & ix( 2 )'", []( auto& expect ) {
    index_type idx;
    idx.add( 1u, 16 );
    idx.add( 2u, 16 );
    idx.add( 2u, 400 );
    idx.add( 1u, 300 );
    idx.add( 3u, 3000 );

    std::byte data[1024];
    auto const len = serialize( data, idx );
    auto const env = environment::builder{}.add( "ix", bytestring_view{ data, len } ).build();

    std::string_view const input{ "ix( 1 ) & ix( 2 )" };
    auto const query = riot::parse( input );
    auto const rs = query->eval( env );

    expect( ! ! rs );
    expect( rs.segment_offset(), equal_to( 0x41414141ull ) );
    expect( rs.values(), equal_to( { 16u } ) );
  } );

  test( "evaluate: 'ix( 1 ) + iy( 2 )'", []( auto& expect ) {
    index_type ix;
    ix.add( 1u, 16 );
    ix.add( 2u, 16 );
    ix.add( 2u, 400 );
    ix.add( 1u, 300 );
    ix.add( 3u, 3000 );

    index_type iy;
    iy.add( 1u, 31 );
    iy.add( 2u, 41 );
    iy.add( 2u, 401 );
    iy.add( 1u, 301 );
    iy.add( 3u, 3001 );

    std::byte data_x[1024];
    auto const len_x = serialize( data_x, ix );
    std::byte data_y[1024];
    auto const len_y = serialize( data_y, iy );

    auto const env = //
        environment::builder{}
            .add( "ix", bytestring_view{ data_x, len_x } )
            .add( "iy", bytestring_view{ data_y, len_y } )
            .build();

    std::string_view const input{ "ix( 1 ) + iy( 2 )" };
    auto const query = riot::parse( input );
    auto const rs = query->eval( env );

    expect( ! ! rs );
    expect( rs.segment_offset(), equal_to( 0x41414141ull ) );
    expect( rs.values(), equal_to( { 16u, 41u, 300u, 401u } ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
