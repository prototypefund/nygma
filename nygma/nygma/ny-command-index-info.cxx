// SPDX-License-Identifier: UNLICENSE

#include <libriot/index-view.hxx>
#include <libunclassified/femtolog.hxx>

#include <nygma/ny-command-index-info.hxx>
#include <nygma/ny-command-support.hxx>

#include <chrono>
#include <cstdint>
#include <iterator>
#include <map>

extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
}

namespace nygma {

void ny_command_index_info( index_info_config const& config ) {

  flog( lvl::i, "index_info.path = ", config._path );

  auto iv = riot::make_poly_index_view( config._path );

  std::cout << "index_view.path = " << config._path << std::endl
            << "index_view.segment_offset = " << iv->segment_offset() << std::endl
            << "index_view.keys = {";
  iv->output_keys( std::cout );
  std::cout << "}" << std::endl;
}

} // namespace nygma
