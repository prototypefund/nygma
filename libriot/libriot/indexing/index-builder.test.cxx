// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>
#include <pest/xoshiro.hxx>

#include <libriot/indexing/index-builder.hxx>

#include <cassert>
#include <map>
#include <sstream>

namespace {

template <typename K, typename V>
using map_type = std::map<K, V>;

struct text_serialzer {

  std::ostream& _os;

  text_serialzer( std::ostream& os ) : _os{ os } {}

  template <typename T, std::size_t BlockLen>
  void encode( T const* p, std::size_t const n ) noexcept {
    _os << '[' << n << "] ";
    for( unsigned i = 0; i < n; i++ ) {
      _os << p[i] << ( i == ( n - 1 ) ? "" : " " );
    }
  }

  template <typename T, std::size_t BlockLen>
  void encode_cblock( T const* p, std::size_t const n, bool const begin ) noexcept {
    _os << "<cbloc" << ( begin ? '-' : '+' );
    encode<T, BlockLen>( p, n );
    _os << '>' << std::endl;
  }

  template <typename T, std::size_t BlockLen>
  void encode_oblock( T const* p, std::size_t const n ) noexcept {
    _os << "<oblock";
    encode<T, BlockLen>( p, n );
    _os << '>' << std::endl;
  }

  template <typename T, std::size_t BlockLen>
  void encode_kblock( T const* p, std::size_t const n ) noexcept {
    _os << "<kblock";
    encode<T, BlockLen>( p, n );
    _os << '>' << std::endl;
  }

  template <typename KeyType>
  void encode_mblock( std::uint32_t const kb, std::uint32_t const ob, std::uint64_t const ) noexcept {
    _os << "<mblock _offset_kblock=" << kb << " _offset_oblock=" << ob << '>' << std::endl;
  }

  std::uint32_t current_position() noexcept { return static_cast<std::uint32_t>( _os.tellp() ); }
};

emptyspace::pest::suite basic( "index-builder basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "chunked_vector: insert single value", []( auto& expect ) {
    riot::chunked_vector<std::uint32_t, 128> cs;
    cs.push( 2342 );

    expect( cs.size(), equal_to( 1u ) );
    expect( cs.chunk_count(), equal_to( 1u ) );
    expect( cs.chunk_offset(), equal_to( 1u ) );
    expect( cs.chunk( 0 )[0], equal_to( 2342u ) );
  } );

  test( "chunked_vector: fill one chunk", []( auto& expect ) {
    auto xo = emptyspace::xoshiro::xoshiro128starstar32{ 2342 };
    auto xs = std::vector<std::uint32_t>{};
    riot::chunked_vector<std::uint32_t, 128> cs;

    for( unsigned i = 0; i < 128; i++ ) {
      auto x = xo();
      xs.push_back( x );
      cs.push( x );
    }

    for( unsigned i = 0; i < 128; i++ ) {
      expect( cs.at( i ), equal_to( xs.at( i ) ) );
    }

    expect( cs.size(), equal_to( 128u ) );
    expect( cs.chunk_count(), equal_to( 1u ) );
    expect( cs.chunk_offset(), equal_to( 0u ) );

    cs.push( 2342 );
    expect( cs.size(), equal_to( 129u ) );
    expect( cs.chunk_count(), equal_to( 2u ) );
    expect( cs.chunk_offset(), equal_to( 1u ) );
    expect( cs.at( 128 ), equal_to( 2342u ) );
    expect( cs.chunk( 1 )[0], equal_to( 2342u ) );
  } );

  test( "chunked_vector: fill multiple chunks", []( auto& expect ) {
    auto xo = emptyspace::xoshiro::xoshiro128starstar32{ 2342 };
    auto xs = std::vector<std::uint32_t>{};
    riot::chunked_vector<std::uint32_t, 128> cs;

    for( unsigned i = 0; i < 512; i++ ) {
      auto x = xo();
      xs.push_back( x );
      cs.push( x );
    }

    for( unsigned i = 0; i < 512; i++ ) {
      expect( cs.at( i ), equal_to( xs.at( i ) ) );
    }

    expect( cs.size(), equal_to( 512u ) );
    expect( cs.chunk_count(), equal_to( 4u ) );
    expect( cs.chunk_offset(), equal_to( 0u ) );
  } );

  test( "index_builder: add two values", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 0x23421337u, 16 );
    idx.add( 0x13372342u, 24 );

    expect( idx.key_count(), equal_to( 2u ) );
  } );

  test( "index_builder: add same value twice", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 0x23421337u, 16 );
    idx.add( 0x23421337u, 16 );

    std::ostringstream os;
    text_serialzer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 1u ) );
    expect( os.str(), equal_to( R"(<cbloc-[1] 16>
<kblock[1] 591532855>
<oblock[1] 0>
<mblock _offset_kblock=15 _offset_oblock=37>
)" ) );
  } );

  test( "index_builder accepts serializer", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );

    std::ostringstream os;
    text_serialzer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 2u ) );
    expect( os.str(), equal_to( R"(<cbloc-[1] 24>
<cbloc-[1] 16>
<kblock[2] 13372342 23421337>
<oblock[2] 0 15>
<mblock _offset_kblock=30 _offset_oblock=60>
)" ) );
  } );

  test( "index_builder accepts serializer 2", []( auto& expect ) {
    using index_type = riot::index_builder<std::uint32_t, map_type, 128>;
    index_type idx;

    idx.add( 23421337u, 16 );
    idx.add( 13372342u, 24 );
    idx.add( 23421337u, 400 );
    idx.add( 1u, 300 );
    idx.add( 13372342u, 3000 );

    std::ostringstream os;
    text_serialzer ser{ os };
    idx.accept( ser, 0u );

    expect( idx.key_count(), equal_to( 3u ) );
    expect( os.str(), equal_to( R"(<cbloc-[1] 300>
<cbloc-[2] 24 3000>
<cbloc-[2] 16 400>
<kblock[3] 1 13372342 23421337>
<oblock[3] 0 16 36>
<mblock _offset_kblock=55 _offset_oblock=87>
)" ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
