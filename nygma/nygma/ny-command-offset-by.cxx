// SPDX-License-Identifier: UNLICENSE

#include <libnygma/dissect.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/indexing/index-view.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <nygma/ny-command-offset-by.hxx>

#include <chrono>
#include <cstdint>
#include <map>

extern "C" {
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
}

namespace nygma {

struct index_file_dependencies {

  std::vector<std::filesystem::path> _i4;
  std::vector<std::filesystem::path> _i6;
  std::vector<std::filesystem::path> _ix;
  std::vector<std::filesystem::path> _iy;

  void gather( std::filesystem::path const& root, std::filesystem::path const& strem );
};

void index_file_dependencies::gather(
    std::filesystem::path const& root, std::filesystem::path const& stem ) {
  auto const prefix = stem.string();
  for( auto& f : std::filesystem::directory_iterator( root ) ) {
    std::error_code ec;
    if( not f.is_regular_file( ec ) ) { continue; }
    auto const p = f.path();
    flog( lvl::d, "visting file = ", p );
    if( not p.has_extension() ) { continue; }
    if( not p.string().starts_with( prefix ) ) { continue; }
    auto const ext = p.extension();
    if( ext == ".i4" ) {
      _i4.push_back( p );
    } else if( ext == ".i6" ) {
      _i6.push_back( p );
    } else if( ext == ".ix" ) {
      _ix.push_back( p );
    } else if( ext == ".iy" ) {
      _iy.push_back( p );
    }
  }
  flog( lvl::i, "index_file_dependencies._count_i4 = ", _i4.size() );
  flog( lvl::i, "index_file_dependencies._count_i6 = ", _i6.size() );
  flog( lvl::i, "index_file_dependencies._count_ix = ", _ix.size() );
  flog( lvl::i, "index_file_dependencies._count_iy = ", _iy.size() );
}

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
    flog( lvl::i, "executing query = i4(", config._key_i4, ") (", key, ")" );
    for( auto& p : deps._i4 ) {
      auto iv = riot::make_poly_index_view( p );
      iv->query( key, offsets );
      std::cout << "segment offset = " << iv->segment_offset() << std::endl;
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_i6.empty() ) {
    __uint128_t key;
    if( auto const rc = ::inet_pton(AF_INET6, config._key_i6.c_str(), &key); rc <= 0) {
      flog( lvl::e, "invalid i6 key = ", config._key_i6 );
      throw std::runtime_error( "invalid i6 key");
    }
    flog( lvl::i, "executing query = i6(", config._key_i6, ")");
    for( auto& p : deps._i6 ) {
      auto iv = riot::make_poly_index_view( p );
      iv->query_ex( key, offsets );
      std::cout << "segment offset = " << iv->segment_offset() << std::endl;
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_ix.empty() ) {
    auto const key = static_cast<std::uint32_t>( std::stoul( config._key_ix ) );
    flog( lvl::i, "executing query = ix(", config._key_ix, ") (", key, ")" );
    for( auto& p : deps._ix ) {
      auto iv = riot::make_poly_index_view( p );
      iv->query( key, offsets );
      std::cout << "segment offset = " << iv->segment_offset() << std::endl;
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
  if( not config._key_iy.empty() ) {
    auto const key = static_cast<std::uint32_t>( std::stoul( config._key_iy ) );
    flog( lvl::i, "executing query = iy(", config._key_iy, ") (", key, ")" );
    for( auto& p : deps._iy ) {
      auto iv = riot::make_poly_index_view( p );
      iv->query( key, offsets );
      std::cout << "segment offset = " << iv->segment_offset() << std::endl;
      std::ostream_iterator<std::uint32_t> out{ std::cout, "\n" };
      std::copy_n( offsets.begin(), std::min( offsets.size(), per_index_limit ), out );
    }
  }
}

} // namespace nygma
