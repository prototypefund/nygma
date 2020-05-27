// SPDX-License-Identifier: BlueOak-1.0.0

#include <filesystem>
#include <string_view>
#include <tuple>
#include <vector>

namespace nygma {

struct index_file_dependencies {

  std::vector<std::filesystem::path> _i4;
  std::vector<std::filesystem::path> _i6;
  std::vector<std::filesystem::path> _ix;
  std::vector<std::filesystem::path> _iy;

  void gather( std::filesystem::path const& root, std::filesystem::path const& strem );

  template <typename F>
  void for_each( F const f ) {
    auto const sz = _i4.size();
    if( _ix.size() != sz ) {
      throw std::runtime_error( "index_file_dependencies::for_each: number of index files differ" );
    }
    for( std::size_t i = 0; i < sz; i++ ) { f( std::forward_as_tuple( _i4[i], _ix[i] ) ); }
  }

  template <typename F>
  void for_each_y( F const f ) {
    auto const sz = _i4.size();
    if( _ix.size() != sz or _iy.size() != sz ) {
      throw std::runtime_error( "index_file_dependencies::for_each_y: number of index files differ" );
    }
    for( std::size_t i = 0; i < sz; i++ ) { f( std::forward_as_tuple( _i4[i], _ix[i], _iy[i] ) ); }
  }
};

} // namespace nygma
