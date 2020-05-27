// SPDX-License-Identifier: BlueOak-1.0.0

#include <libnygma/dissect.hxx>
#include <libnygma/pcap-reassembler.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/index-view.hxx>
#include <libriot/query-evaluator.hxx>
#include <libriot/query-parser.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <nygma/ny-command-query.hxx>
#include <nygma/ny-command-support.hxx>

#include <chrono>
#include <cstdint>
#include <map>

namespace nygma {

void ny_command_query( query_config const& config ) {

  auto const d = config._root == "" ? config._path.parent_path() : config._root;
  auto const f = config._path.filename().stem();
  auto const expected_base = d / f;

  flog( lvl::i, "query_config.root = ", d );
  flog( lvl::i, "query_config.expected_base = ", expected_base );
  flog( lvl::i, "query_config.query = ", config._query );

  index_file_dependencies deps;
  deps.gather( d, expected_base );

  auto const query = riot::parse( config._query );

  auto data = std::make_unique<block_view_16k>( config._path, block_flags::rd );
  nygma::pcap::with( std::move( data ), [&]( auto& pcap ) {
    if( not pcap.valid() ) {
      flog( lvl::e, "unable to open pcap storage path = ", config._path );
      return;
    }

    // open output pcap or `stdout`
    auto os = config._out == "-" ? nygma::pcap_ostream{ STDOUT_FILENO }
                                 : nygma::pcap_ostream{ config._out };

    pcap::reassemble_begin( pcap, os );

    deps.for_each( [&]( auto const index_files ) {
      auto [i4, ix] = index_files;
      auto const env = riot::environment::builder().add( "i4", i4 ).add( "ix", ix ).build();
      auto const rs = query->eval( env );
      pcap::reassemble_stream( pcap, rs.segment_offset(), rs.cbegin(), rs.cend(), os );
    } );
  } );
}

} // namespace nygma
