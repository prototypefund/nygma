// License: RoundRobin-1.0.0

#include <libnygma/dissect.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/index-builder.hxx>
#include <libriot/index-compressor.hxx>
#include <libriot/index-cycler.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <tetch/t3-command-index.hxx>
#include <tetch/t3-hyperscan.hxx>
#include <tetch/t3-index-trace.hxx>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <map>

namespace t3tch {

using hash_type = nygma::dissect::void_hash_policy;
template <typename K, typename V>
using map_type = std::map<K, V>;
using index_iy_type = typename riot::index_builder<std::uint32_t, map_type, 128>;
using index_trace_type = typename t3tch::index_trace<index_iy_type, hs_engine>;

template <template <typename> typename S1>
struct poly_cycler {
  std::string _name;
  riot::index_cycler _cyc;
  template <typename... Args>
  poly_cycler( std::string_view const name, Args&&... args )
    : _name{ name }, _cyc{ std::forward<Args>( args )... } {}
  template <typename I>
  void operator()( I&& i, std::uint64_t const o ) noexcept {
    flog( lvl::m, "cycler{", _name, "} index path = ", _cyc.path() );
    flog( lvl::m, "cycler{", _name, "} index.keys = ", i->key_count(), " index.segment_offset = ", o );
    _cyc.accept<S1>( std::move( i ), o );
  }
};

using c128 = poly_cycler<riot::svb128d1_serializer>;

void t3_command_index_pcap( index_pcap_config const& config ) {
  // the async index writer, it is shared among all cyclers
  auto const w = std::make_shared<riot::index_writer>();

  auto const d = config._path.parent_path();
  auto const f = config._path.filename().stem();

  flog( lvl::m, "cycler.directory = ", d );
  flog( lvl::m, "cycler.filestem = ", f );

  c128 cyc{ "iy", w, d, f, ".iy" };

  auto const cycler = [&]( std::unique_ptr<index_iy_type> iy,
                           std::uint64_t const segment_offset ) noexcept {
    flog( lvl::i, "cycler callback for segment offset = ", segment_offset );
    cyc( std::move( iy ), segment_offset );
  };

  std::size_t total_packets{ 0 };
  std::size_t total_bytes{ 0 };

  flog( lvl::m, "pcap storage path = ", config._path );

  auto is = std::ifstream{ config._patterns };
  auto const p = ioc::parse( is );

  flog( lvl::m, "pattern_database.size = ", p->size() );

  auto const pattern_filter_stateless = [&mode = config._mode]( auto const& pattern_type ) {
    return mode == "pure" ? pattern_type == ioc::pattern_type::PURE
                          : pattern_type == ioc::pattern_type::REGEXP;
  };

  auto const engine_mode_stateless = config._mode == "pure" ? hs_engine_mode::PURE
                                                            : hs_engine_mode::REGEXP;

  auto const pattern_filter_dns = []( auto const& pattern_type ) {
    return pattern_type == ioc::pattern_type::PURE_DOMAIN;
  };

  flog( lvl::m, "compiling pattern-match engines" );

  index_trace_type trace{ hs_engine{ engine_mode_stateless, p, pattern_filter_stateless },
                          hs_engine{ hs_engine_mode::PUREV, p, pattern_filter_dns } };

  flog( lvl::m, "starting indexing" );

  hash_type hash;
  std::uint64_t hits = 0;

  auto data = std::make_unique<nygma::block_view_2m>( config._path, nygma::block_flags::rd );

  auto const start = std::chrono::high_resolution_clock::now();

  nygma::pcap::with( std::move( data ), [&]( auto& pcap ) {
    if( not pcap.valid() ) {
      flog( lvl::e, "invalid pcap" );
      return;
    }
    pcap.for_each( [&]( auto const& pkt, auto const offset ) noexcept {
      trace.prepare( offset, cycler );
      t3tch::dissect::dissect_en10mb( hash, trace, pkt._slice );
      total_packets++;
      total_bytes += pkt._slice.size();
      hits += trace._matched_ids.size();
    } );
    trace.finish( cycler );
  } );

  auto const end = std::chrono::high_resolution_clock::now();

  auto const delta_t = std::chrono::duration<double>( end - start ).count();
  auto const rate_packtes = to_Mops( total_packets, delta_t );
  auto const rate_bits = to_Mbps( total_bytes, delta_t );

  flog( lvl::m, "hits = ", hits );
  flog( lvl::i, "delta_t = ", delta_t );
  flog( lvl::i, "total bytes = ", total_bytes );
  flog( lvl::i, "rate packets = ", rate_packtes, "Mpps" );
  flog( lvl::i, "rate bits = ", rate_bits, "Mbps" );
  flog( lvl::i, "v4 packet count = ", trace._v4_count );
  flog( lvl::i, "v6 packet count = ", trace._v6_count );
  flog( lvl::i, "udp packet count = ", trace._udp_count );
  flog( lvl::i, "tcp packet count = ", trace._tcp_count );
  flog( lvl::i, "dns packet count = ", trace._dns_count );
  flog( lvl::i, "total packet count = ", total_packets );
} // namespace t3tch

} // namespace t3tch
