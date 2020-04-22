// SPDX-License-Identifier: UNLICENSE

#include <algorithm>
#include <argh/argh.hxx>

#include <cctype>
#include <libnygma/version.hxx>
#include <libriot/version.hxx>
#include <libunclassified/femtolog.hxx>

#include <nygma/ny-command-index.hxx>

#include <cstdlib>
#include <exception>
#include <iostream>

using namespace emptyspace;
using namespace unclassified;
using namespace nygma;

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
argh::ValueFlag<lvl::type, verbosity_reader> verbosity(
    arguments, "integer", "verbosity level", { 'v', "verbosity" } );

void ny_show_version() {
  flog( lvl::i, "ny libriot.version = ", LIBRIOT_VERSION_STR );
  flog( lvl::i, "ny libnygma.version = ", LIBNYGMA_VERSION_STR );
}

//--indexing-a-pcap------------------------------------------------------------

void ny_command_index_pcap( argh::Subparser& argh ) {
  auto const methods = "NONE|BITPACK|STREAMVBYTE";
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::ValueFlag<std::string> method_4( argh, "compression-method", methods, { "i4" }, "BITPACK" );
  argh::ValueFlag<std::string> method_6( argh, "compression-method", methods, { "i6" }, "BITPACK" );
  argh::ValueFlag<std::string> method_x( argh, "compression-method", methods, { "ix" }, "BITPACK" );
  argh::Positional<std::string> path( argh, "path", "the pcap to index" );

  argh.Parse();

  auto const to_method = []( std::string const& method ) -> compression_method {
    std::string M = method;
    std::transform( M.begin(), M.end(), M.begin(), []( auto const c ) { return std::toupper( c ); } );
    if( M == "NONE" ) {
      return compression_method::NONE;
    } else if( M == "BITPACK" ) {
      return compression_method::BITPACK;
    } else if( M == "STREAMVBYTE" ) {
      return compression_method::STREAMVBYTE;
    } else {
      throw argh::ValidationError( "invalid compression-method" );
    }
  };

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }

  ny_show_version();

  index_pcap_config config;
  config._path = argh::get( path );
  config._method_i4 = to_method( argh::get( method_4 ) );
  config._method_i6 = to_method( argh::get( method_6 ) );
  config._method_ix = to_method( argh::get( method_x ) );

  flog( lvl::i, "index_pcap_config._path = ", config._path );
  flog( lvl::i, "index_pcap_config._method_i4 = ", to_string( config._method_i4 ) );
  flog( lvl::i, "index_pcap_config._method_i6 = ", to_string( config._method_i6 ) );
  flog( lvl::i, "index_pcap_config._method_ix = ", to_string( config._method_ix ) );

  ny_index_pcap( config );
}

//--show-version---------------------------------------------------------------

void ny_command_version( argh::Subparser& argh ) {
  argh.Parse();
  femtolog::log::instance().level( lvl::i );
  ny_show_version();
}

} // namespace

int main( int argc, char* argv[] ) {

  argh::ArgumentParser argh( "ny index, query & reassembly of pcaps" );
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Group commands( argh, "commands" );
  argh::Command index( commands, "index-pcap", "index a pcap file", &ny_command_index_pcap );
  argh::Command version( commands, "version", "show version", &ny_command_version );
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
