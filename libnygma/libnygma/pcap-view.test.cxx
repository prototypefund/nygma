// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libnygma/pcap-view.hxx>

#include <algorithm>

namespace pcap = nygma::pcap;

namespace {

struct expected_format {
  std::uint32_t _magic{ 0 };
  std::uint32_t _linktype{ static_cast<std::uint32_t>( pcap::linktype::en10mb ) };
  std::uint16_t _version_major{ 2 };
  std::uint16_t _version_minor{ 4 };
  std::uint32_t _sigfigs{ 0 };
  std::uint32_t _thiszone{ 0 };
  std::uint32_t _snaplen;

  expected_format( pcap::format::type const format, std::uint32_t const snaplen )
    : _snaplen{ snaplen } {
    _magic = format;
  }
};

class pcap_consumer {
 public:
  emptyspace::pest::test_state& expect;
  expected_format const& _format;

  std::size_t _count{ 0 };
  std::size_t _total_bytes{ 0 };
  std::uint64_t _last_stamp{ std::numeric_limits<std::uint64_t>::max() };
  std::size_t _last_size{ std::numeric_limits<std::size_t>::max() };

 public:
  pcap_consumer( emptyspace::pest::test_state& env, expected_format const& format )
    : expect{ env }, _format{ format } {}

  template <typename View>
  inline void operator()( View const& view ) {
    using namespace emptyspace::pest;
    expect( view._raw_magic, equal_to( _format._magic ) );
    expect( view._raw_linktype, equal_to( _format._linktype ) );
    expect( view._raw_snaplen, equal_to( _format._snaplen ) );
    expect( view._raw_version_minor, equal_to( _format._version_minor ) );
    expect( view._raw_version_major, equal_to( _format._version_major ) );
    expect( view._raw_sigfigs, equal_to( _format._sigfigs ) );
    expect( view._raw_thiszone, equal_to( _format._thiszone ) );

    expect( view.valid(), equal_to( true ) );

    for( auto const& pkt : view ) {
      _count++;
      _last_stamp = pkt._stamp;
      _total_bytes += pkt._slice.size();
      _last_size = pkt._slice.size();
    }
  }
};

static unsigned char pcap_le_en10mb_1[] = {
    0xd4, 0xc3, 0xb2, 0xa1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
};
static_assert( sizeof( pcap_le_en10mb_1 ) == 24 );

static unsigned char pcap_be_en10mb_1[] = {
    0xa1, 0xb2, 0xc3, 0xd4, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
};
static_assert( sizeof( pcap_be_en10mb_1 ) == 24 );

static unsigned char pcap_le_en10mb_with_data_1[] = {
    0xd4, 0xc3, 0xb2, 0xa1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static_assert( sizeof( pcap_le_en10mb_with_data_1 ) == 24 + 16 );

static unsigned char pcap_le_en10mb_with_data_2[] = {
    0xd4, 0xc3, 0xb2, 0xa1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static_assert( sizeof( pcap_le_en10mb_with_data_2 ) == 24 + 16 );

static unsigned char pcap_nsec_le_en10mb_with_data_1[] = {
    0x4d, 0x3c, 0xb2, 0xa1, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static_assert( sizeof( pcap_nsec_le_en10mb_with_data_1 ) == 24 + 16 );

emptyspace::pest::suite basic_mmap( "pcap in-memory suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace nygma;

  test( "little-endian", []( auto& expect ) {
    bytestring_view data{ pcap_le_en10mb_1 };
    expected_format f{ pcap::format::PCAP_USEC, 65535 };
    pcap_consumer c{ expect, f };
    pcap::with( data, c );
    expect( c._count, equal_to( 0u ) );
  } );

  test( "big-endian", []( auto& expect ) {
    bytestring_view data{ pcap_be_en10mb_1 };
    expected_format f{ pcap::format::PCAP_USEC, 65535 };
    pcap_consumer c{ expect, f };
    pcap::with( data, c );
    expect( c._count, equal_to( 0u ) );
  } );

  test( "little-endian-packet1", []( auto& expect ) {
    bytestring_view data{ pcap_le_en10mb_with_data_1 };
    expected_format f{ pcap::format::PCAP_USEC, 65535 };
    pcap_consumer c{ expect, f };
    pcap::with( data, c );
    expect( c._count, equal_to( 1u ) );
    expect( c._last_stamp, equal_to( 0u ) );
    expect( c._last_size, equal_to( 0u ) );
  } );

  test( "little-endian-packet2", []( auto& expect ) {
    bytestring_view data{ pcap_le_en10mb_with_data_2 };
    expected_format f{ pcap::format::PCAP_USEC, 65535 };
    pcap_consumer c{ expect, f };
    pcap::with( data, c );
    expect( c._count, equal_to( 1u ) );
    expect( c._last_stamp, equal_to( 1000000u ) );
    expect( c._last_size, equal_to( 0u ) );
  } );

  test( "little-endian-nsec-packet1", []( auto& expect ) {
    bytestring_view data{ pcap_nsec_le_en10mb_with_data_1 };
    expected_format f{ pcap::format::PCAP_NSEC, 65535 };
    pcap_consumer c{ expect, f };
    pcap::with( data, c );
    expect( c._count, equal_to( 1u ) );
    expect( c._last_stamp, equal_to( 1u ) );
    expect( c._last_size, equal_to( 0u ) );
  } );
} );

emptyspace::pest::suite basic_blockio( "pcap blockio suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace nygma;

  test( "block view: random-2k.bin", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-2k.bin", block_flags::rdd };
    auto const bs = bv.prefetch( 0 );
    expect( bs.size(), equal_to( ( 2ul << 10 ) ) );
  } );

  test( "block view: random-2k.bin with invalid offset", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-2k.bin", block_flags::rdd };
    auto const bs = bv.prefetch( 4ul << 10 );
    expect( bs.size(), equal_to( 0u ) );
  } );

  test( "block view: random-4k.bin", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-4k.bin", block_flags::rdd };
    auto const bs = bv.prefetch( 0 );
    expect( bs.size(), equal_to( ( 4ul << 10 ) ) );
  } );

  test( "block view: random-4k.bin with offset=2k", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-4k.bin", block_flags::rdd };
    auto const bs = bv.prefetch( 2ul << 10 );
    expect( bs.size(), equal_to( ( 2ul << 10 ) ) );
  } );

  test( "block view: random-2m.bin", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-2m.bin", block_flags::rdd };
    auto const bs = bv.prefetch( 0 );
    expect( bs.size(), equal_to( ( 2ul << 20 ) ) );
  } );

  test( "open random-4m.bin", []( auto& expect ) {
    block_view_2m bv{ "tests/data/random-4m.bin", block_flags::rdd };
    auto const bs1 = bv.prefetch( 0 );
    expect( bs1.size(), equal_to( ( 2ul << 20 ) ) );
    auto const bs2 = bv.prefetch( 2ul << 20 );
    expect( bs2.size(), equal_to( ( 2ul << 20 ) ) );
    expect( bs1.data(), equal_to( bs2.data() ) );
  } );

  test( "block view: dns-over-tcp6.pcap", []( auto& expect ) {
    block_view_2m bv{ "tests/data/pcap/dns-over-tcp6.pcap", block_flags::rdd };
    auto const bs = bv.prefetch( 0 );
    expect( bs.size(), equal_to( 2235u ) );
  } );

  test( "parse pcap from `block_view` dns-over-tcp6.pcap", []( auto& expect ) {
    auto bv = std::make_unique<block_view_2m>( "tests/data/pcap/dns-over-tcp6.pcap", block_flags::rdd );
    auto const bs = bv->prefetch( 0 );
    expect( bs.size(), equal_to( 2235u ) );
    expected_format f{ pcap::format::PCAP_USEC, 524288 };
    pcap_consumer c{ expect, f };
    pcap::with( std::move( bv ), c );
    expect( c._count, equal_to( 10u ) );
  } );

  test( "parse pcap from `mmap_view` dns-over-tcp6.pcap", []( auto& expect ) {
    mmap_view mv{ "tests/data/pcap/dns-over-tcp6.pcap" };
    auto const bs = mv.view();
    expect( bs.size(), equal_to( 2235u ) );
    expected_format f{ pcap::format::PCAP_USEC, 524288 };
    pcap_consumer c{ expect, f };
    pcap::with( bs, c );
    expect( c._count, equal_to( 10u ) );
  } );

  test( "slice first packet using `block_view{ 1000.pcap }`", []( auto& expect ) {
    auto bv = std::make_unique<block_view_2m>( "tests/data/pcap/1000.pcap", block_flags::rd );
    pcap::with( std::move( bv ), [&]( auto& pcap ) {
      auto const pkt = pcap.slice( pcap::PCAP_HEADERSZ + pcap::PACKET_HEADERSZ );
      expect( pkt.size(), equal_to( 75u ) );
      expect( pkt.stamp(), equal_to( 1424219007658518000ull ) );
    } );
  } );

  struct query {
    std::uint32_t _offset;
    std::size_t _size;
    std::uint64_t _timestamp;

    constexpr query( std::uint32_t offset, std::size_t size, std::uint64_t timestamp )
      : _offset{ offset }, _size{ size }, _timestamp{ timestamp } {}

    query( query&& ) noexcept = default;
  };

  test( "bulk replay&slice `block_view{ 1000.pcap }`", []( auto& expect ) {
    std::vector<query> queries;
    {
      auto bv = std::make_unique<block_view_2m>( "tests/data/pcap/1000.pcap", block_flags::rd );
      pcap::with( std::move( bv ), [&]( auto& pcap ) {
        expect( pcap.valid(), equal_to( true ) );
        if( not pcap.valid() ) { return; }
        pcap.for_each( [&]( auto& pkt, auto const offset ) {
          queries.emplace_back( offset, pkt.size(), pkt.stamp() );
        } );
      } );
    }
    auto bv = std::make_unique<block_view_2m>( "tests/data/pcap/1000.pcap", block_flags::rd );
    pcap::with( std::move( bv ), [&]( auto& pcap ) {
      for( auto& q : queries ) {
        auto const pkt = pcap.slice( q._offset );
        expect( pkt.size(), equal_to( q._size ) );
        expect( pkt.stamp(), equal_to( q._timestamp ) );
      }
    } );
  } );

  test( "bulk slice `block_view{ 1000.pcap }`", []( auto& expect ) {
    query queries[] = {
        // generated using the indexer ( with limit )
        { 40, 75u, 1424219007658518000ull },     { 131, 75u, 1424219007658559000ull },
        { 222, 95u, 1424219007737404000ull },    { 333, 95u, 1424219007737414000ull },
        { 444, 62u, 1424219007760103000ull },    { 522, 62u, 1424219007760112000ull },
        { 600, 82u, 1424219007800276000ull },    { 698, 82u, 1424219007800286000ull },
        { 796, 60u, 1424219007801348000ull },    { 872, 54u, 1424219007801358000ull },
        { 942, 62u, 1424219007821570000ull },    { 1020, 62u, 1424219007821579000ull },
        { 1244, 62u, 1424219007872018000ull },   { 1322, 62u, 1424219007872027000ull },
        { 1400, 101u, 1424219007873054000ull },  { 1517, 101u, 1424219007873061000ull },
        { 2072, 80u, 1424219007954612000ull },   { 2168, 80u, 1424219007954621000ull },
        { 2410, 1514u, 1424219008033493000ull }, { 3940, 1514u, 1424219008033503000ull },
        { 5470, 1514u, 1424219008033512000ull }, { 7000, 1514u, 1424219008033514000ull },
        { 8530, 1514u, 1424219008033703000ull },
    };
    auto bv = std::make_unique<block_view_2m>( "tests/data/pcap/1000.pcap", block_flags::rd );
    pcap::with( std::move( bv ), [&]( auto& pcap ) {
      for( auto& q : queries ) {
        auto const pkt = pcap.slice( q._offset );
        expect( pkt.size(), equal_to( q._size ) );
        expect( pkt.stamp(), equal_to( q._timestamp ) );
      }
    } );
  } );
} );
} // namespace

int main() {
  basic_mmap( std::clog );
  basic_blockio( std::clog );
  return ( EXIT_SUCCESS );
}
