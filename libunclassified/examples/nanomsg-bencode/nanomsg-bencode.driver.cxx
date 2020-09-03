// SPDX-License-Identifier: BlueOak-1.0.0

#include <argh/argh.hxx>

#include <libunclassified/backoff-strategy.hxx>
#include <libunclassified/bencode.hxx>
#include <libunclassified/femtolog.hxx>
#include <libunclassified/nanomsg.hxx>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string_view>
#include <thread>

using namespace unclassified::nanomsg;
namespace femtolog = unclassified::femtolog;
namespace bencode = unclassified::bencode;

using build = bencode::build;

namespace {

namespace fs = std::filesystem;
using nanomsg_type = nanomsg<1024, 2048>;
using backoff_strategy = unclassified::backoff_strategy::backoff3;

inline void server_impl( std::string const& path, unsigned const nworkers ) {
  flog( lvl::i, "workers = ", nworkers );
  flog( lvl::i, "shm-path = ", std::string_view{ path.c_str() } );

  try {
    nanomsg_type ctx{ path, mode::create };
    unsigned int msgs = 0;
    auto& ring = ctx.ring();
    while( true ) {
      auto idx = ring.read_idx();
      std::array<std::byte, 512> const* p{ nullptr };
      for( backoff_strategy bo{}; ( p = ring.read_ptr<std::array<std::byte, 512>>( idx ) ) == nullptr;
           ++bo )
        bo();
      std::array<std::byte, 512> const& msg = *p;
      auto data = bencode::dstream{ msg };
      auto key = std::string_view{};
      auto value = std::string_view{};
      data >> build::dict >> key >> value >> build::end;
      if( data._failed ) {
        flog( lvl::w, "invalid message" );
      } else {
        flog( lvl::m, "message: ", msgs, " key = ", key, " data = ", value );
      }
      ring.read_commit( idx );
      msgs++;
    }
  } catch( ... ) { flog( lvl::e, "unable to create shm queue path = ", path ); }
}

inline void client_impl( std::string const& path, std::string_view const key,
                         std::string_view const data ) {
  try {
    nanomsg_type ctx{ path, mode::attach };

    flog( lvl::m, "key = ", key );
    flog( lvl::m, "data = ", data );

    auto& ring = ctx.ring();

    ring.push<backoff_strategy>( [&]( auto* p ) noexcept {
      auto& msg = *reinterpret_cast<std::array<std::byte, 512>*>( p );
      auto build = bencode::build{ msg };
      build << build::dict << build::key{ key } << data << build::end;
    } );

  } catch( ... ) { flog( lvl::e, "unable to attach to shm queue path = ", path ); }
}

} // namespace

int main( int const argc, char const** argv ) {
  using namespace emptyspace::argh;
  ArgumentParser argh( "nanomsg & bencode client server example" );
  HelpFlag help( argh, "help", "display help", { 'h', "help" } );
  Group commands( argh, "commands" );
  Command client( commands, "client", "start client to send message" );
  Command server( commands, "server", "start server message loop" );
  ValueFlag<std::string> path( argh, "path", "/example-nanomsg-bencode.1", { 'p', "shm-path" } );
  ValueFlag<unsigned> workers( server, "workers", "1", { 'n', "workers" } );
  ValueFlag<std::string> key( client, "key", "test-key", { "key" } );
  ValueFlag<std::string> data( client, "data", "test-data", { "data" } );
  //if( help ) {
  //  std::cerr << argh << std::endl;
  //  return EXIT_SUCCESS;
  //}
  try {
    argh.ParseCLI( argc, argv );
    if( server ) {
      server_impl( path.Get(), workers.Get() );
    } else {
      client_impl( path.Get(), key.Get(), data.Get() );
    }
  } catch( Help const& ) { //
    std::cerr << argh;
  } catch( Error const& e ) {
    std::cerr << "error: " << e.what() << std::endl << argh;
  } catch( std::exception const& e ) { //
    std::cerr << "error: " << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
