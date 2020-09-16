// License: RoundRobin-1.0.0

#pragma once

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <hs/hs.h>

#ifdef __clang__
#  pragma clang diagnostic pop
#endif

#include <libioc/pattern-parser.hxx>

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace t3tch {

enum class hs_engine_mode { PURE, PUREV, REGEXP, REGEXPV };

struct hs_engine {
 private:
  hs_database_t* _database{ nullptr };
  hs_scratch_t* _scrtch{ nullptr };

 public:
  template <typename Filter>
  explicit hs_engine( hs_engine_mode const mode, ioc::pattern_database const& patterns,
                      Filter const select ) {
    std::vector<char const*> ps;
    std::vector<std::size_t> ls;
    std::vector<hs_expr_ext> es;
    std::vector<unsigned> ids;
    std::vector<unsigned> fs;
    hs_platform_info_t platform;
    platform.cpu_features = HS_CPU_FEATURES_AVX2;
    platform.tune = HS_TUNE_FAMILY_GENERIC;

    for( auto const& p : patterns._patterns ) {
      auto const& pattern = p->second.find( "pattern" );
      if( pattern == p->second.end() ) { throw std::runtime_error( "hs_engine: invalid patterns" ); }
      if( mode == hs_engine_mode::PURE or mode == hs_engine_mode::PUREV ) {
        auto const& type = p->second.find( "pattern_type" );
        if( type == p->second.end() or not select( type->second ) ) { continue; }
        ids.push_back( p->first );
        ps.push_back( pattern->second.c_str() );
        ls.push_back( pattern->second.size() );
        fs.push_back( HS_FLAG_SINGLEMATCH );
      } else {
        auto const& type = p->second.find( "pattern_type" );
        if( type == p->second.end() or not select( type->second ) ) { continue; }
        ids.push_back( p->first );
        ps.push_back( pattern->second.c_str() );
        ls.push_back( pattern->second.size() );
        es.push_back( to_extended_flags( p->second ) );
        fs.push_back( to_flags( p->second ) | HS_FLAG_SINGLEMATCH );
      }
    }

    if( mode == hs_engine_mode::PURE or mode == hs_engine_mode::PUREV ) {
      hs_compile_error_t* cerr{ nullptr };
      if( auto const rc = hs_compile_lit_multi( ps.data(), fs.data(), ids.data(), ls.data(),
                                                static_cast<unsigned>( ps.size() ),
                                                mode == hs_engine_mode::PURE ? HS_MODE_BLOCK
                                                                             : HS_MODE_VECTORED,
                                                &platform, &_database, &cerr );
          rc != HS_SUCCESS ) {
        if( cerr not_eq nullptr ) { hs_free_compile_error( cerr ); }
        throw std::runtime_error( "hs_engine::constructor: unable to compile pure(v) patterns" );
      }
    } else {
      // extended flags go into a separate container
      std::vector<hs_expr_ext const*> es_ptrs;
      for( unsigned i = 0; i < es.size(); i++ ) { es_ptrs[i] = &es[i]; }
      hs_compile_error_t* cerr{ nullptr };
      if( auto const rc = hs_compile_ext_multi( ps.data(), fs.data(), ids.data(), es_ptrs.data(),
                                                static_cast<unsigned>( ps.size() ),
                                                mode == hs_engine_mode::REGEXP ? HS_MODE_BLOCK
                                                                               : HS_MODE_VECTORED,
                                                &platform, &_database, &cerr );
          rc != HS_SUCCESS ) {
        if( cerr not_eq nullptr ) { hs_free_compile_error( cerr ); }
        throw std::runtime_error( "hs_engine::constructor: unable to compile regexp(v) patterns" );
      }
    }

    if( auto const rc = hs_alloc_scratch( _database, &_scrtch ); rc != HS_SUCCESS ) {
      hs_free_database( _database );
      throw std::runtime_error( "hs_engine::constructor: unable to allocate scratch space" );
    }
  }

  hs_engine( hs_engine const& ) = delete;
  hs_engine& operator=( hs_engine const& ) = delete;

  hs_engine( hs_engine&& o ) noexcept : _database{ nullptr }, _scrtch{ nullptr } { o.swap( *this ); }
  hs_engine& operator=( hs_engine&& o ) noexcept {
    o.swap( *this );
    return *this;
  }

  static hs_expr_ext to_extended_flags( ioc::pattern_database::pattern_data const& pattern ) noexcept {
    hs_expr_ext ext;
    ext.flags = 0;
    ext.min_offset = 0;
    ext.max_offset = 0;
    ext.edit_distance = 0;
    ext.hamming_distance = 0;
    ext.min_length = 0;
    for( auto const& p : pattern ) {
      if( p.first == "min_end" ) {
        ext.flags |= HS_EXT_FLAG_MIN_OFFSET;
        ext.min_offset = std::stoull( p.second );
      } else if( p.first == "max_end" ) {
        ext.flags |= HS_EXT_FLAG_MAX_OFFSET;
        ext.max_offset = std::stoull( p.second );
      } else if( p.first == "min_len" ) {
        ext.flags |= HS_EXT_FLAG_MIN_LENGTH;
        ext.min_length = std::stoull( p.second );
      } else if( p.first == "edit_dist" ) {
        ext.flags |= HS_EXT_FLAG_EDIT_DISTANCE;
        ext.edit_distance = static_cast<unsigned>( std::stoul( p.second ) );
      } else if( p.first == "hamming_dist" ) {
        ext.flags |= HS_EXT_FLAG_HAMMING_DISTANCE;
        ext.edit_distance = static_cast<unsigned>( std::stoull( p.second ) );
      }
    }
    return ext;
  }

  static unsigned to_flags( ioc::pattern_database::pattern_data const& pattern ) noexcept {
    unsigned flags{ 0 };
    for( auto const& p : pattern ) {
      if( p.first == "i" ) {
        flags |= HS_FLAG_CASELESS;
      } else if( p.first == "s" ) {
        flags |= HS_FLAG_DOTALL;
      } else if( p.first == "m" ) {
        flags |= HS_FLAG_MULTILINE;
      } else if( p.first == "p" ) {
        flags |= HS_FLAG_PREFILTER;
      } else if( p.first == "c" ) {
        flags |= HS_FLAG_COMBINATION;
      }
    }
    return flags;
  }

  void status_to( std::ostream& os ) {
    os << "hs_engine._regexp_database.size = ";
    if( std::size_t sz{ 0 }; hs_database_size( _database, &sz ) == HS_SUCCESS ) {
      os << sz << std::endl;
    } else {
      os << "<error>" << std::endl;
    }
    os << "hs_engine._regexp_scrtch.size = ";
    if( std::size_t sz{ 0 }; hs_scratch_size( _scrtch, &sz ) == HS_SUCCESS ) {
      os << sz << std::endl;
    } else {
      os << "<error>" << std::endl;
    }
  }

  void swap( hs_engine& o ) noexcept {
    using std::swap;
    swap( _database, o._database );
    swap( _scrtch, o._scrtch );
  }

  ~hs_engine() {
    if( _scrtch not_eq nullptr ) { hs_free_scratch( _scrtch ); }
    if( _database not_eq nullptr ) { hs_free_database( _database ); }
  }

  static int on_match( unsigned const id, unsigned long long const, unsigned long long const,
                       unsigned const, void* arg ) noexcept {
    auto* ids = static_cast<std::vector<ioc::pattern_database::pattern_id>*>( arg );
    ids->push_back( id );
    return 0;
  }

  bool scan( std::byte const* const p, std::size_t const sz,
             std::vector<ioc::pattern_database::pattern_id>& ids ) {
    auto const* data = reinterpret_cast<char const*>( p );
    auto const rc = hs_scan( _database, data, static_cast<unsigned>( sz ), 0, _scrtch, on_match,
                             std::addressof( ids ) );
    return rc == HS_SUCCESS;
  }

  bool scanv( std::byte const* const* p, unsigned int* sz, std::size_t const count,
              std::vector<ioc::pattern_database::pattern_id>& ids ) {
    auto const data = reinterpret_cast<char const* const*>( p );
    auto const rc = hs_scan_vector( _database, data, sz, static_cast<unsigned>( count ), 0, _scrtch,
                                    on_match, std::addressof( ids ) );
    return rc == HS_SUCCESS;
  }
}; // namespace t3tch

} // namespace t3tch
