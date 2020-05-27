// SPDX-License-Identifier: BlueOak-1.0.0

#include <argh/argh.hxx>
#include <libnygma/version.hxx>
#include <libriot/version.hxx>
#include <libunclassified/femtolog.hxx>

#include <nygma/ny-command-index-info.hxx>
#include <nygma/ny-command-index.hxx>
#include <nygma/ny-command-offset-by.hxx>
#include <nygma/ny-command-query.hxx>
#include <nygma/ny-command-reverse-slice-by.hxx>
#include <nygma/ny-command-slice-by.hxx>

#include <algorithm>
#include <cctype>
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

void ny_index_pcap( argh::Subparser& argh ) {
  auto const methods = "NONE|BITPACK|STREAMVBYTE";
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::ValueFlag<std::string> method_4( argh, "compression", methods, { "i4" }, "STREAMVBYTE" );
  argh::ValueFlag<std::string> method_6( argh, "compression", methods, { "i6" }, "STREAMVBYTE" );
  argh::ValueFlag<std::string> method_x( argh, "compression", methods, { "ix" }, "STREAMVBYTE" );
  argh::Positional<std::string> path( argh, "path", "path to the pcap to index" );

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

  ny_command_index_pcap( config );
}

//--querying-offsets-----------------------------------------------------------

void ny_offsets_by( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the indexed pcap" );
  argh::ValueFlag<std::string> root( argh, "directory", "root path override", { "root" } );
  argh::ValueFlag<std::string> k4( argh, "ipv4 address", "the i4 key", { "i4" } );
  argh::ValueFlag<std::string> k6( argh, "ipv6 address", "the i6 key", { "i6" } );
  argh::ValueFlag<std::string> kx( argh, "port number", "the ix key", { "ix" } );
  argh::ValueFlag<std::string> ky( argh, "match id", "the iy key", { "iy" } );

  argh.Parse();

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }

  offsets_by_config config;
  config._path = argh::get( path );
  config._root = argh::get( root );
  config._key_i4 = argh::get( k4 );
  config._key_i6 = argh::get( k6 );
  config._key_ix = argh::get( kx );
  config._key_iy = argh::get( ky );

  ny_show_version();

  flog( lvl::i, "offsets_by_config._path = ", config._path );
  flog( lvl::i, "offsets_by_config._root = ", config._root );
  flog( lvl::i, "offsets_by_config._key_i4 = ", config._key_i4 );
  flog( lvl::i, "offsets_by_config._key_i6 = ", config._key_i6 );
  flog( lvl::i, "offsets_by_config._key_ix = ", config._key_ix );
  flog( lvl::i, "offsets_by_config._key_iy = ", config._key_iy );

  ny_command_offset_by( config );
}

//--slice-a-pcap-(-unsorted-output-)------------------------------------------

void ny_slice_by( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the indexed pcap" );
  argh::ValueFlag<std::string> root( argh, "directory", "root path override", { "root" } );
  argh::ValueFlag<std::string> out( argh, "output", "the restitched output", { 'o', "output" }, "-" );
  argh::ValueFlag<std::string> k4( argh, "ipv4 address", "the i4 key", { "i4" } );
  argh::ValueFlag<std::string> k6( argh, "ipv6 address", "the i6 key", { "i6" } );
  argh::ValueFlag<std::string> kx( argh, "port number", "the ix key", { "ix" } );
  argh::ValueFlag<std::string> ky( argh, "match id", "the iy key", { "iy" } );

  argh.Parse();

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }

  slice_config config;
  config._path = argh::get( path );
  config._root = argh::get( root );
  config._out = argh::get( out );
  config._key_i4 = argh::get( k4 );
  config._key_i6 = argh::get( k6 );
  config._key_ix = argh::get( kx );
  config._key_iy = argh::get( ky );

  ny_show_version();

  flog( lvl::i, "slice_config._path = ", config._path );
  flog( lvl::i, "slice_config._root = ", config._root );
  flog( lvl::i, "slice_config._out = ", config._out );
  flog( lvl::i, "slice_config._key_i4 = ", config._key_i4 );
  flog( lvl::i, "slice_config._key_i6 = ", config._key_i6 );
  flog( lvl::i, "slice_config._key_ix = ", config._key_ix );
  flog( lvl::i, "slice_config._key_iy = ", config._key_iy );

  ny_command_slice_by( config );
}

//--query-and-restitch-pcap---------------------------------------------------

void ny_query( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the indexed pcap" );
  argh::ValueFlag<std::string> root( argh, "directory", "root path override", { "root" } );
  argh::ValueFlag<std::string> out( argh, "output", "the restitched output", { 'o', "output" }, "-" );
  argh::ValueFlag<std::string> query( argh, "query-expression", "the query", { 'q', "query" } );

  argh.Parse();

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }

  query_config config;
  config._path = argh::get( path );
  config._root = argh::get( root );
  config._out = argh::get( out );
  config._query = argh::get( query );

  ny_show_version();

  flog( lvl::i, "query_config._path = ", config._path );
  flog( lvl::i, "query_config._root = ", config._root );
  flog( lvl::i, "query_config._out = ", config._out );
  flog( lvl::i, "query_config._query = ", config._query );

  ny_command_query( config );
}

//--reverse-slicing-----------------------------------------------------------

void ny_reverse_slice( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the indexed pcap" );
  argh::ValueFlag<std::string> root( argh, "directory", "root path override", { "root" } );
  argh::ValueFlag<std::string> out( argh, "output", "the restitched output", { 'o', "output" }, "-" );
  argh::ValueFlag<std::string> ky( argh, "match id", "the iy key", { "iy" } );

  argh.Parse();

  if( not path ) { throw argh::Help( "path to pcap file missing" ); }

  reverse_slice_config config;
  config._path = argh::get( path );
  config._root = argh::get( root );
  config._out = argh::get( out );
  config._key_iy = argh::get( ky );

  ny_show_version();

  flog( lvl::i, "reverse_slice_config._path = ", config._path );
  flog( lvl::i, "reverse_slice_config._root = ", config._root );
  flog( lvl::i, "reverse_slice_config._out = ", config._out );
  flog( lvl::i, "reverse_slice_config._query = ", config._key_iy );

  ny_command_reverse_slice_by( config );
}

//--index-info----------------------------------------------------------------

void ny_index_info( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::ValueFlag<std::string> index( argh, "path", "path to the index file", { 'i', "index" } );

  argh.Parse();

  if( not index ) { throw argh::Help( "path to index file missing" ); }

  index_info_config config;
  config._path = argh::get( index );

  ny_show_version();

  flog( lvl::i, "index_info_config._path = ", config._path );

  ny_command_index_info( config );
}

//--show-version---------------------------------------------------------------

void ny_version( argh::Subparser& argh ) {
  argh.Parse();
  femtolog::log::instance().level( lvl::i );
  ny_show_version();
}

} // namespace

int main( int argc, char* argv[] ) {
  argh::ArgumentParser argh( "ny index, query & reassembly of pcaps" );
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Group commands( argh, "commands" );
  argh::Command index( commands, "index-pcap", "index a pcap file", &ny_index_pcap );
  argh::Command offsets( commands, "offsets-by", "query offsets", &ny_offsets_by );
  argh::Command slice( commands, "slice-by", "restitch pcap from query", &ny_slice_by );
  argh::Command query( commands, "query", "restitch pcap from query", &ny_query );
  argh::Command version( commands, "version", "show version", &ny_version );
  argh::Command info( commands, "index-info", "show info about index file", &ny_index_info );
  argh::Command
      reverse( commands, "reverse-slice-by", "restitch pcap from reverse query", &ny_reverse_slice );
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
