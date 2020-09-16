// License: RoundRobin-1.0.0

#include <the-0wls/0wl-command-parse.hxx>
#include <the-0wls/0wl-parse-pdml.hxx>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace owl {

void owl_command_parse( parse_config const& config ) {

  using namespace xml;

  try {
    auto const packets = owl::pdml::parse( config._path );

    constexpr std::size_t INDENT = 2;
    struct print {
      void operator()( owl::pdml::field const& f ) const noexcept {
        std::cout << std::setw( 4 ) << f._level << " " << std::string( INDENT * f._level, ' ' )
                  << f._text << std::endl;
      }
      void operator()( owl::pdml::proto const& p ) const noexcept {
        std::cout << std::setw( 4 ) << p._level << " " << std::string( INDENT * p._level, ' ' )
                  << p._text << std::endl;
      }
    };
    std::cout << "packets.size = " << packets.size() << std::endl;
    print print;
    for( auto& packet : packets ) {
      for( auto& node : packet._nodes ) { node->accept( print ); }
    }
  } catch( std::exception const& e ) { std::cerr << e.what() << std::endl; }
}

} // namespace owl
