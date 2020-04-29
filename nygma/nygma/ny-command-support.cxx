// SPDX-License-Identifier: UNLICENSE

#include <libunclassified/femtolog.hxx>

#include <nygma/ny-command-support.hxx>

#include <algorithm>
#include <filesystem>

namespace nygma {

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

  std::sort( _i4.begin(), _i4.end() );
  std::sort( _i6.begin(), _i6.end() );
  std::sort( _ix.begin(), _ix.end() );
  std::sort( _iy.begin(), _iy.end() );

  flog( lvl::i, "index_file_dependencies._count_i4 = ", _i4.size() );
  flog( lvl::i, "index_file_dependencies._count_i6 = ", _i6.size() );
  flog( lvl::i, "index_file_dependencies._count_ix = ", _ix.size() );
  flog( lvl::i, "index_file_dependencies._count_iy = ", _iy.size() );
}

} // namespace nygma
