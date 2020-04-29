// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/pcap-reassembler.hxx>
#include <libnygma/pcap-view.hxx>

namespace {

emptyspace::pest::suite basic( "pcap reassembler suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace nygma;

  struct query {
    std::uint32_t _offset;
    std::size_t _size;
    std::uint64_t _timestamp;

    constexpr query( std::uint32_t offset, std::size_t size, std::uint64_t timestamp )
      : _offset{ offset }, _size{ size }, _timestamp{ timestamp } {}

    query( query&& ) noexcept = default;
  };

  test( "slice and reassemble pcap using predefined offsets", []( auto& expect ) {
    query queries[] = {
        // generated using the indexer ( with limit )
        { 40, 75u, 1424219007658518000ull },
        { 222, 95u, 1424219007737404000ull },
        { 444, 62u, 1424219007760103000ull },
        { 600, 82u, 1424219007800276000ull },
    };
    auto vs = std::vector<std::uint32_t>{};
    { // iterate and collect / check offsets into 1000.pcap
      auto bv = std::make_unique<block_view>( "tests/data/pcap/1000.pcap", block_flags::rd );
      pcap::with( std::move( bv ), [&]( auto& pcap ) {
        for( auto& q : queries ) {
          vs.push_back( q._offset );
          auto const pkt = pcap.slice( q._offset );
          expect( pkt.size(), equal_to( q._size ) );
          expect( pkt.stamp(), equal_to( q._timestamp ) );
        }
      } );
    }
    { // reassemble pcap using the given offsets
      auto os = pcap_ostream{ "./pcap-reassembler.test.pcap" };
      auto bv = std::make_unique<block_view>( "tests/data/pcap/1000.pcap", block_flags::rd );
      pcap::with( std::move( bv ), [&]( auto& pcap ) {
        expect( pcap.valid(), equal_to( true ) );
        pcap::reassemble_from( pcap, vs.cbegin(), vs.cend(), os );
      } );
    }
    std::error_code ec;
    std::filesystem::path p{ "./pcap-reassembler.test.pcap" };
    auto const size = std::filesystem::file_size( p, ec );
    expect( not ec, equal_to( true ) );
    expect(
        size,
        equal_to( pcap::PCAP_HEADERSZ + 4u * pcap::PACKET_HEADERSZ + 75u + 95u + 62u + 82u ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
