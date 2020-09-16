// License: RoundRobin-1.0.0

#include <argh/argh.hxx>
#include <libunclassified/femtolog.hxx>

#include <imgui/imgui.h>

#include <the-0wls/0wl-command-parse.hxx>
#include <the-0wls/0wl-command-ui.hxx>
#include <the-0wls/0wl-vulkan.hxx>

#include <array>
#include <cstdlib>
#include <iostream>

namespace {

using namespace emptyspace;

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
    unclassified::femtolog::log::instance().level( out );
  }
};

argh::Group arguments( "arguments" );
argh::ValueFlag<lvl::type, verbosity_reader> verbosity( arguments, "integer", "verbosity level",
                                                        { 'v', "verbosity" } );

void owl_show_version() { flog( lvl::m, "imgui.version = ", "unknown"); }

void owl_ui( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the workspace directory",
                                      "/non-existent" );

  argh.Parse();

  owl_show_version();

  owl::ui_config config;
  config._workspace_directory = argh::get( path );

  owl::owl_command_ui( config );
}

void owl_parse( argh::Subparser& argh ) {
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Positional<std::string> path( argh, "path", "path to the pml" );

  argh.Parse();

  owl_show_version();

  owl::parse_config config;
  config._path = argh::get( path );

  owl::owl_command_parse( config );
}

} // namespace

int main( int argc, char* argv[] ) {
  argh::ArgumentParser argh( "the-0wls -- three-penny-imgui --" );
  argh::HelpFlag help( argh, "help", "show this help message", { 'h', "help" } );
  argh::Group commands( argh, "commands" );
  argh::Positional<std::string> path( argh, "path", "path a `pdml` file" );
  argh::Command ui( commands, "ui", "start dear-imgui", &owl_ui );
  argh::Command pdml( commands, "parse-pdml", "parse a pdml file", &owl_parse );
  argh::GlobalOptions globals( argh, arguments );

  // index-info: key | size table
  // index-pcap
  // index-lookup ( ix( ... ) )
  // sparse scan
  // session selector table
  // regexp text field
  // tshark pdml parser

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

  return EXIT_SUCCESS;
}
