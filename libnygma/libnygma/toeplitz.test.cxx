// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/toeplitz.hxx>

#include <array>
#include <cstdint>
#include <type_traits>

extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
}

namespace {

namespace toeplitz = emptyspace::toeplitz;
using ip6_addr_t = std::array<std::byte, 16>;

struct dir {
  enum {
    ips,
    ipd,
  };
};

union ipv4 {
  struct {
    std::uint32_t _ip[2];
  };
  std::array<std::byte, 8> _array;
};

union ipv6 {
  struct {
    in6_addr _ip[2];
  };
  std::array<std::byte, 32> _array;
};

static_assert( sizeof( ipv4 ) == 8 );
static_assert( sizeof( ipv6 ) == 32 );

template <std::size_t N, std::size_t M>
static constexpr ipv4 make_v4( char const ( &src )[N], char const ( &dst )[M] ) {
  auto data = ipv4{};
  data._ip[dir::ips] = ::inet_addr( src );
  data._ip[dir::ipd] = ::inet_addr( dst );
  return ( data );
}

template <std::size_t N, std::size_t M>
static constexpr ipv6 make_v6( char const ( &src )[N], char const ( &dst )[M] ) {
  auto data = ipv6{};
  ::inet_pton( AF_INET6, src, &data._ip[dir::ips] );
  ::inet_pton( AF_INET6, dst, &data._ip[dir::ipd] );
  return ( data );
}

emptyspace::pest::suite basic( "toeplitz suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "v4: toeplitz_scalar_lut<sym> 00", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_symmetric> rss;
    auto data = make_v4( "127.0.0.1", "127.0.0.1" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x0u ) );
  } );

  test( "v4: toeplitz_scalar_lut<sym> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_symmetric> rss;
    auto data = make_v4( "66.9.149.187", "161.142.100.80" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 173607513u ) );
  } );

  test( "v4: toeplitz_scalar_lut<ms> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "66.9.149.187", "161.142.100.80" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x323e8fc2u ) );
  } );

  test( "v4: toeplitz_scalar_lut<ms> 02", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "199.92.111.2", "65.69.140.83" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0xd718262au ) );
  } );

  test( "v4: toeplitz_scalar_lut<ms> 03", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "24.19.198.95", "12.22.207.184" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0xd2d0a5deu ) );
  } );

  test( "v4: toeplitz_scalar_lut<ms> 04", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "38.27.205.30", "209.142.163.6" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x82989176u ) );
  } );

  test( "v4: toeplitz_scalar_lut<ms> 05", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "153.39.163.191", "202.188.127.2" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x5d1809c5u ) );
  } );

  test( "v4: toeplitz_scalar_loop<sym> 00", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_symmetric> rss;
    auto data = make_v4( "127.0.0.1", "127.0.0.1" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x0u ) );
  } );

  test( "v4: toeplitz_scalar_loop<sym> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_symmetric> rss;
    auto data = make_v4( "66.9.149.187", "161.142.100.80" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 173607513u ) );
  } );

  test( "v4: toeplitz_scalar_loop<ms> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "66.9.149.187", "161.142.100.80" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x323e8fc2u ) );
  } );

  test( "v4: toeplitz_scalar_loop<ms> 02", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "199.92.111.2", "65.69.140.83" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0xd718262au ) );
  } );

  test( "v4: toeplitz_scalar_loop<ms> 03", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "24.19.198.95", "12.22.207.184" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0xd2d0a5deu ) );
  } );

  test( "v4: toeplitz_scalar_loop<ms> 04", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "38.27.205.30", "209.142.163.6" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x82989176u ) );
  } );

  test( "v4: toeplitz_scalar_loop<ms> 05", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v4( "153.39.163.191", "202.188.127.2" );
    auto hash = rss.hash<8>( data._array.data() );
    expect( hash, equal_to( 0x5d1809c5u ) );
  } );

  test( "v6: toeplitz_scalar_lut<ms> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:2501:200:1fff::7", "3ffe:2501:200:3::1" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x2cc18cd5u ) );
  } );

  test( "v6: toeplitz_scalar_lut<ms> 02", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:501:8::260:97ff:fe40:efab", "ff02::1" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x0f0c461cu ) );
  } );

  test( "v6: toeplitz_scalar_lut<ms> 03", []( auto& expect ) {
    toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:1900:4545:3:200:f8ff:fe21:67cf", "fe80::200:f8ff:fe21:67cf" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x4b61e985u ) );
  } );

  test( "v6: toeplitz_scalar_loop<ms> 01", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:2501:200:1fff::7", "3ffe:2501:200:3::1" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x2cc18cd5u ) );
  } );

  test( "v6: toeplitz_scalar_loop<ms> 02", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:501:8::260:97ff:fe40:efab", "ff02::1" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x0f0c461cu ) );
  } );

  test( "v6: toeplitz_scalar_loop<ms> 03", []( auto& expect ) {
    toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_ms> rss;
    auto data = make_v6( "3ffe:1900:4545:3:200:f8ff:fe21:67cf", "fe80::200:f8ff:fe21:67cf" );
    auto hash = rss.hash<32>( data._array.data() );
    expect( hash, equal_to( 0x4b61e985u ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
