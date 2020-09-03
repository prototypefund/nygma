// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libunclassified/nanomsg.hxx>
#include <libunclassified/support.hxx>

#include <cstring>
#include <filesystem>
#include <future>

namespace {

emptyspace::pest::suite basic( "nanomsg suite", []( auto& test ) {
  namespace fs = std::filesystem;
  using namespace emptyspace::pest;
  using namespace unclassified::nanomsg;

  constexpr std::size_t MESSAGECNT = 1024;
  constexpr std::size_t MESSAGESZ = 2048;
  constexpr std::byte data[MESSAGESZ]{ std::byte( 0x41u ) };

  using nanomsg_type = nanomsg<MESSAGECNT, MESSAGESZ>;

  test( "create nanomsg context", []( auto& expect ) {
    fs::path path{ "/test-nanomsg-ring.1" };
    bool threw = false;
    try {
      nanomsg_type server( path, mode::create );
      nanomsg_type client( path, mode::attach );
      unclassified::unused( server, client );
    } catch( ... ) { threw = true; }
    expect( threw, equal_to( false ) );
  } );

  test( "message ping pong: server <-> client", [&]( auto& expect ) {
    fs::path path{ "/test-nanomsg-ring.2" };
    {
      nanomsg_type server( path, mode::create );
      nanomsg_type client( path, mode::attach );

      {
        auto& ring = server.ring();
        auto idx = ring.write_idx();
        std::byte* p;
        while( ( p = ring.write_ptr<std::byte>( idx ) ) == nullptr )
          ;
        std::memcpy( p, data, sizeof( data ) );
        ring.write_commit( idx );
      }

      {
        auto& ring = client.ring();
        auto idx = ring.read_idx();
        std::byte const* p;
        while( ( p = ring.read_ptr<std::byte>( idx ) ) == nullptr )
          ;
        expect( std::memcmp( p, data, sizeof( data ) ), equal_to( 0 ) );
        ring.read_commit( idx );
      }
    }
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
