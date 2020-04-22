// SPDX-License-Identifier: UNLICENSE

#include <cstdint>
#include <libnygma/dissect.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/indexing/index-builder.hxx>
#include <libriot/indexing/index-compressor.hxx>
#include <libriot/indexing/index-cycler.hxx>
#include <libriot/indexing/index-trace.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <nygma/ny-command-index.hxx>

#include <chrono>
#include <map>
#include "libriot/indexing/index-serializer.hxx"

namespace nygma {

using hash_type = dissect::void_hash_policy;
template <typename K, typename V>
using map_type = std::map<K, V>;
using index_i4_type = typename riot::index_builder<std::uint32_t, map_type, 256>;
using index_ix_type = typename riot::index_builder<std::uint32_t, map_type, 128>;
using index_trace_type = typename riot::index_trace<index_i4_type, index_ix_type>;

template <
    template <typename>
    typename S1,
    template <typename>
    typename S2,
    template <typename>
    typename S3>
struct poly_cycler {
  std::string _name;
  riot::index_cycler _cyc;
  compression_method const _method;
  template <typename... Args>
  poly_cycler( std::string_view const name, compression_method const method, Args&&... args )
    : _name{ name }, _cyc{ std::forward<Args>( args )... }, _method{ method } {}
  template <typename I>
  void operator()( I&& i, std::uint64_t segment_offset ) noexcept {
    flog( lvl::i, "cycler{", _name, "} index path = ", _cyc.path() );
    flog( lvl::i, "cycler{", _name, "} index.keys = ", i->key_count() );
    switch( _method ) {
      case compression_method::NONE: _cyc.accept<S1>( std::move( i ), segment_offset ); break;
      case compression_method::BITPACK: _cyc.accept<S2>( std::move( i ), segment_offset ); break;
      case compression_method::STREAMVBYTE: _cyc.accept<S3>( std::move( i ), segment_offset ); break;
    }
  }
};

using poly256 =
    poly_cycler<riot::uc256_serializer, riot::bp256d1_serializer, riot::svb256d1_serializer>;
using poly128 =
    poly_cycler<riot::uc128_serializer, riot::bp128d1_serializer, riot::svb128d1_serializer>;

void ny_index_pcap( index_pcap_config const& config ) {
  // the async index writer, it is shared among all cyclers
  auto const w = std::make_shared<riot::index_writer>();

  auto const d = config._path.parent_path();
  auto const f = config._path.filename().stem();

  flog( lvl::i, "cycler.directory = ", d );
  flog( lvl::i, "cycler.filestem = ", f );

  poly256 cyc4{ "i4", config._method_i4, w, d, f, ".i4" };
  poly128 cycx{ "ix", config._method_i6, w, d, f, ".ix" };

  auto const cycler = [&]( std::unique_ptr<index_i4_type> i4,
                     std::unique_ptr<index_ix_type> ix,
                     std::uint64_t const segment_offset ) noexcept {
    flog( lvl::i, "cycler callback for segment offset = ", segment_offset );
    cyc4( std::move( i4 ), segment_offset );
    cycx( std::move( ix ), segment_offset );
  };

  std::size_t total_packets{ 0 };
  std::size_t total_bytes{ 0 };
  std::uint64_t first_seen{ std::numeric_limits<std::uint64_t>::max() };
  std::uint64_t last_seen{ 0 };

  auto data = std::make_unique<nygma::block_view>( config._path, nygma::block_flags::rd );
  auto const start = std::chrono::high_resolution_clock::now();

  index_trace_type trace;
  hash_type hash;
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
  auto const delta_t = std::chrono::duration<double>( end - start ).count();
  char first[unclassified::format::TIMESTAMP_BUFSZ];
  char last[unclassified::format::TIMESTAMP_BUFSZ];
  auto const nf = unclassified::format::format_ts( first, first_seen );
  auto const nl = unclassified::format::format_ts( last, last_seen );
  auto const rate_packtes = to_Mops( total_packets, delta_t );
  auto const rate_bits = to_Mbps( total_bytes, delta_t );

  flog( lvl::i, "delta_t = ", delta_t );
  flog( lvl::i, "total bytes = ", total_bytes );
  flog( lvl::i, "rate packets = ", rate_packtes, "Mpps" );
  flog( lvl::i, "rate bits = ", rate_bits, "Mbps" );
  flog( lvl::i, "first seen = ", std::string_view{ first, nf } );
  flog( lvl::i, "last seen = ", std::string_view{ last, nl } );
  flog( lvl::i, "v4 packet count = ", trace._v4_count );
  flog( lvl::i, "v6 packet count = ", trace._v6_count );
  flog( lvl::i, "total packet count = ", total_packets );
}

} // namespace nygma
