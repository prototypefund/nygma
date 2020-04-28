# NYGMA - network packet processing

... here be dragons -- prototype ahead ...

project `nygma` is organized as a set of ( mostly ) header only `cxx` libraries as well as an example
console application `ny`.

instead of boring you to death the obligatory animated gif:

## ny

basic command line demonstration tool for indexing and reassembling pcaps

![ny](https://64k.by/assets/nygma.gif)

## libnygma

base functionality and dealing with network packets

  - [x] rss hashing in software ( using toeplitz hashing )
  - [x] lightweight packet dissector
  - [x] lightweight dns dissector

## libriot

(r)apid (i)ndexing (o)f network (t)raffic ( or data thereof ).

indexing, querying and index serialization using SIMD based integer compression.

features & current status:

  - [x] indexing of existing pcap's inplace
      - [x] provide indexing method for ipv4 addresses
      - [x] provide indexing method for ipv6 addresses
      - [x] provide indexing method for ports
      - [x] index compression using SIMD bitpacking/streamvbyte
      - [ ] provide indexing for *IOC*s ( regexp that is basically )
  - [x] provide means of querying the index
  - [x] reassemble a pcap given an index query ( and the original pcap )
  - [ ] lightweight query engine supporting SIMD accelerated union and intersection ops
  - [ ] ... fun ...

## libcapture ( TBD )

full packet capture ( or caching ) and compressed capture storage format ( `ccap` ).

  - [ ] combined full packet capture / caching & indexing on commodity ( server ) hardware
  - [ ] a storage format suited for network packets and indexing
      - [ ] full 64bit timestamps in nanosecond resolution
      - [ ] block oriented and optimized for indexing
      - [ ] integer compression for internal block offsets ...
      - [ ] stores capture port for each packet

## example / code snippets

@see: [ny-command-index.cxx](https://github.com/stackless-goto/nygma/blob/master/nygma/nygma/ny-command-index.cxx)

```cpp
...

using hash_type = dissect::void_hash_policy;
template <typename K, typename V>
using map_type = std::map<K, V>;
using index_i4_type = typename riot::index_builder<std::uint32_t, map_type, 256>;
using index_ix_type = typename riot::index_builder<std::uint32_t, map_type, 128>;
using index_trace_type = typename riot::index_trace<index_i4_type, index_ix_type>;

auto data = std::make_unique<nygma::block_view>( "test.pcap", nygma::block_flags::rd );
auto const start = std::chrono::high_resolution_clock::now();

...

auto const cycler = [&]( std::unique_ptr<index_i4_type> i4,
                         std::unique_ptr<index_ix_type> ix,
                         std::uint64_t const segment_offset ) noexcept {
  flog( lvl::i, "cycler callback for segment offset = ", segment_offset );
  cyc4( std::move( i4 ), segment_offset );
  cycx( std::move( ix ), segment_offset );
};

index_trace_type trace; 
hash_type hash;         // for rss hashing ( in this case void policy is used )
nygma::pcap::with( std::move( data ), [&]( auto& pcap ) {
  if( not pcap.valid() ) {
    flog( lvl::e, "invalid pcap" );
    return;
  }
  pcap.for_each( [&]( auto const& pkt, auto const offset ) noexcept {
    trace.prepare( offset, cycler );
    riot::dissect::dissect_en10mb( hash, trace, pkt._slice );
    total_packets++;
    total_bytes += pkt._slice.size();
    first_seen = std::min( pkt._stamp, first_seen );
    last_seen = std::max( pkt._stamp, last_seen );
  } );
  trace.finish( cycler );
} );

auto const end = std::chrono::high_resolution_clock::now();

...
```

a unit test for the reassembler using the [~stackless-goto/pest](https://github.com/stackless-goto/pest) helper.

@see: [pcap-reassembler.test.cxx](https://github.com/magenbluten/nygma-staging/blob/master/libnygma/libnygma/pcap-reassembler.test.cxx)

```cpp
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
        { 40, 75u, 1424219665518000000ull },
        { 222, 95u, 1424219744404000000ull },
        { 444, 62u, 1424219767103000000ull },
        { 600, 82u, 1424219807276000000ull },
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
```

## dependencies

`nygma` uses the [build2](https://build2.org) build system to manage all dependencies 
( actually the full project lifecycle ).

  - [~stackless-goto/pest](https://github.com/stackless-goto/pest) for unit testing
  - [~stackless-goto/argh](https://github.com/stackless-goto/argh) for argument parsing in `ny`
  - [~stackless-goto/libforest](https://github.com/stackless-goto/libforest) for `std::map` alternatives and faster index generation
  - [~stackless-goto/libunclassified](https://github.com/stackless-goto/libunclassified) for otherwise unrelated auxiliary functions
  - [build2](https://build2.org) as build system ( [install-doc](https://build2.org/install.xhtml) )

## building & development

development happens mainly on [freebsd](https://freebsd.org). theoretically linux should work
as well. if not it's a bug. please report.

building `ny` example using `clang10`.

```
$ git clone https://github.com/stackless-goto/nygma
$ cd nygma/nygma/
$ bdep init -C @clang10 cc \
    config.cxx=clang++10 \
    config.cxx.poptions="-DNDEBUG" \
    config.cxx.coptions="-flto -stdlib=libc++ -Ofast -fno-rtti -mavx2 -mlzcnt -Wall -Wextra -Wno-c99-designator"
...
$ b
$ nygma/nygma/ny --help
  nygma/nygma/ny COMMAND {OPTIONS}

    ny index, query & reassembly of pcaps

  OPTIONS:

      -h, --help                        show this help message
      commands
        index-pcap                        index a pcap file
        offsets-by                        query offsets
        slice                             slice by given ( simple ) query
        version                           show version
      arguments
        -v[integer],
        --verbosity=[integer]             verbosity level
```

## support & blame-game

the code you find here is probably horrible. there is no excuse to that. therefore, your help is
needed ...

magenbluten < mb [ at ] 64k.by > :: <https://64k.by>

## funding

![bmbf](https://64k.by/assets/bmbf-gefoerdert.jpg)
