// SPDX-License-Identifier: UNLICENSE

#include <libnygma/dissect.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/indexing/index-view.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <nygma/ny-command-offset-by.hxx>
#include <nygma/ny-command-support.hxx>

#include <chrono>
#include <cstdint>
#include <map>

extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
}

namespace nygma {

void ny_command_offset_by( offsets_by_config const& config ) {

  auto const d = config._root == "" ? config._path.parent_path() : config._root;
  auto const f = config._path.filename().stem();
  auto const expected_base = d / f;

  flog( lvl::i, "offset_by.root = ", d );
  flog( lvl::i, "offset_by.expected_base = ", expected_base );
  //flog( lvl::i, "offset_by.pattern = ", "-XXXX.iY" );

  index_file_dependencies deps;
  deps.gather( d, expected_base );

  std::size_t const per_index_limit = 42;
  std::vector<std::uint32_t> offsets;

  if( not config._key_i4.empty() ) {
    auto const key = ntohl( ::inet_addr( config._key_i4.c_str() ) );
    flog( lvl::i, "executing query = i4( ", config._key_i4, " ) ( ", key, " )" );
    for( auto& p : deps._i4 ) {
      flog( lvl::i, "executing query on index file = ", p );
      offsets.clear();
      auto iv = riot::make_poly_index_view( p );
      std::cout << "@segment offset = " << iv->segment_offset() << std::endl;
      iv->query( key, offsets );
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_i6.empty() ) {
    __uint128_t key;
    if( auto const rc = ::inet_pton( AF_INET6, config._key_i6.c_str(), &key ); rc <= 0 ) {
      flog( lvl::e, "invalid i6 key = ", config._key_i6 );
      throw std::runtime_error( "invalid i6 key" );
    }
    flog( lvl::i, "executing query = i6( ", config._key_i6, " )" );
    for( auto& p : deps._i6 ) {
      flog( lvl::i, "executing query on index file = ", p );
      offsets.clear();
      auto iv = riot::make_poly_index_view( p );
      std::cout << "@segment offset = " << iv->segment_offset() << std::endl;
      iv->query_ex( key, offsets );
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_ix.empty() ) {
    auto const key = static_cast<std::uint32_t>( std::stoul( config._key_ix ) );
    flog( lvl::i, "executing query = ix( ", config._key_ix, " ) ( ", key, " )" );
    for( auto& p : deps._ix ) {
      flog( lvl::i, "executing query on index file = ", p );
      offsets.clear();
      auto iv = riot::make_poly_index_view( p );
      std::cout << "@segment offset = " << iv->segment_offset() << std::endl;
      iv->query( key, offsets );
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_iy.empty() ) {
    auto const key = static_cast<std::uint32_t>( std::stoul( config._key_iy ) );
    flog( lvl::i, "executing query = iy( ", config._key_iy, " ) ( ", key, " )" );
    for( auto& p : deps._iy ) {
      flog( lvl::i, "executing query on index file = ", p );
      offsets.clear();
      auto iv = riot::make_poly_index_view( p );
      std::cout << "@segment offset = " << iv->segment_offset() << std::endl;
      iv->query( key, offsets );
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
}

} // namespace nygma
