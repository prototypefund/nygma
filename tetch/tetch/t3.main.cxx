// License: RoundRobin-1.0.0

#include <hs/version.h>

#include <argh/argh.hxx>
#include <libioc/version.hxx>
#include <libnygma/version.hxx>
#include <libriot/version.hxx>

#include <libunclassified/femtolog.hxx>

#include <tetch/t3-command-compile.hxx>
#include <tetch/t3-command-index.hxx>
#include <tetch/t3-index-trace.hxx>

#include <iostream>

using namespace emptyspace;
using namespace unclassified;
using namespace nygma;
using namespace t3tch;

namespace {

lvl::type to_level( unsigned long const verbosity_level ) {
  switch( verbosity_level ) {
    case 0: return lvl::w;
    case 1: return lvl::i;
    case 2: return lvl::v;
    case 3: return lvl::y;
    case 4: return lvl::d;
    default: throw argh::ValidationError( "invalid verbosity level" );
  }
}

struct verbosity_reader {
  void operator()( std::string const&, std::string const& value, lvl::type& out ) {
    out = to_level( std::stoul( value ) );
    femtolog::log::instance().level( out );
  }
};

argh::Group arguments( "arguments" );
argh::ValueFlag<lvl::type, verbosity_reader> verbosity( arguments, "integer", "verbosity level",
                                                        { 'v', "verbosity" } );

void t3_show_version() {
  flog( lvl::i, "t3 libriot.version = ", LIBRIOT_VERSION_STR );
  flog( lvl::i, "t3 libnygma.version = ", LIBNYGMA_VERSION_STR );
  flog( lvl::i, "t3 libhs.version = ", LIBHS_VERSION_STR );
  flog( lvl::i, "t3 libioc.version = ", LIBIOC_VERSION_STR );
}

//--indexing-a-pcap------------------------------------------------------------

void t3_index_pcap( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the pcap to index" );
  argh::ValueFlag<std::string> mode( argh, "mode", "engine mode", { 'm', "mode" }, "pure" );
  argh::ValueFlag<std::string> patterns( argh, "path", "path to the pattern file",
                                         { 'p', "patterns" } );

  argh.Parse();

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }
  if( not patterns ) { throw argh::Help( "path to pattern file missing" ); }

  t3_show_version();

  index_pcap_config config;
  config._path = argh::get( path );
  config._patterns = argh::get( patterns );
  config._mode = argh::get( mode );

  flog( lvl::i, "index_pcap_config._path = ", config._path );
  flog( lvl::i, "index_pcap_config._patterns = ", config._patterns );

  t3_command_index_pcap( config );
}

//--compile-patterns----------------------------------------------------------

void t3_compile_patterns( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::ValueFlag<std::string> patterns( argh, "path", "path to the pattern file", { "patterns" } );
  argh::ValueFlag<std::string> pattern_filter(
      argh, "pattern-filter", "pattern filter: {pure, pure-domain, regexp, regexp-domain}",
      { "pattern-filter" }, "pure" );
  argh::ValueFlag<std::string> mode( argh, "mode", "compilation mode: {pure, purev, regexp, regexpv}",
                                     { "mode" }, "pure" );

  argh.Parse();

  if( not patterns ) { throw argh::Help( "path to pattern file missing" ); }

  t3_show_version();

  compile_patterns_config config;
  config._patterns = argh::get( patterns );
  config._mode = argh::get( mode );
  config._pattern_filter = argh::get( pattern_filter );

  flog( lvl::i, "index_pcap_config._patterns = ", config._patterns );
  flog( lvl::i, "index_pcap_config._mode = ", config._mode );
  flog( lvl::i, "index_pcap_config._pattern_filter = ", config._pattern_filter );

  t3_command_compile_patterns( config );
}
//--show-version---------------------------------------------------------------

void t3_version( argh::Subparser& argh ) {
  argh.Parse();
  femtolog::log::instance().level( lvl::i );
  t3_show_version();
}

} // namespace

int main( int argc, char* argv[] ) {
  argh::ArgumentParser argh( "t3 index pcap using ioc lists" );
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Group commands( argh, "commands" );
  argh::Command index( commands, "index-pcap", "index a pcap file", &t3_index_pcap );
  argh::Command compile( commands, "compile-patterns", "a pattern file", &t3_compile_patterns );
  argh::Command version( commands, "version", "show version", &t3_version );
  argh::GlobalOptions globals( argh, arguments );

  try {
    argh.ParseCLI( argc, argv );
    return EXIT_SUCCESS;
  } catch( argh::Help const& ) {
    std::cerr << argh;
    return EXIT_SUCCESS;
  } catch( argh::ParseError const& e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << argh;
    return EXIT_FAILURE;
  } catch( argh::ValidationError const& e ) {
    std::cerr << e.what() << std::endl;
    std::cerr << argh;
    return EXIT_FAILURE;
  } catch( std::exception const& e ) {
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch( ... ) {
    std::cerr << "error: unknown exception" << std::endl;
    return EXIT_FAILURE;
  }
}
