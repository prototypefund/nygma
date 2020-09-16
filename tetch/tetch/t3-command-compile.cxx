// License: RoundRobin-1.0.0

#include <libioc/pattern-parser.hxx>
#include <libunclassified/femtolog.hxx>

#include <tetch/t3-command-compile.hxx>
#include <tetch/t3-hyperscan.hxx>

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>

namespace t3tch {

void t3_command_compile_patterns( compile_patterns_config const& config ) {
  auto is = std::ifstream{ config._patterns };
  auto const p = ioc::parse( is );

  std::clog << "pattern_datatbase.size = " << p->size() << std::endl;

  auto const pattern_filter = [&filter = config._pattern_filter]( auto const& pattern_type ) {
    if( filter == "pure" ) {
      return pattern_type == ioc::pattern_type::PURE;
    } else if( filter == "pure-domain" ) {
      return pattern_type == ioc::pattern_type::PURE_DOMAIN;
    } else if( filter == "regexp" ) {
      return pattern_type == ioc::pattern_type::REGEXP;
    } else if( filter == "regexp-domain" ) {
      return pattern_type == ioc::pattern_type::REGEXP_DOMAIN;
    }
    return false;
  };

  auto engine_mode = hs_engine_mode::PURE;
  if( config._mode == "pure" ) {
    engine_mode = hs_engine_mode::PURE;
  } else if( config._mode == "purev" ) {
    engine_mode = hs_engine_mode::PUREV;
  } else if( config._mode == "regexp" ) {
    engine_mode = hs_engine_mode::REGEXP;
  } else if( config._mode == "regexpv" ) {
    engine_mode = hs_engine_mode::REGEXPV;
  }

  auto engine = hs_engine{ engine_mode, p, pattern_filter };
  engine.status_to( std::clog );
}

} // namespace t3tch
