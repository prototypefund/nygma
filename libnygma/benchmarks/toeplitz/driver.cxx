// SPDX-License-Identifier: UNLICENSE

#include <pest/pnch.hxx>
#include <pest/xoshiro.hxx>

#include <libnygma/toeplitz.hxx>

#include <array>
#include <iostream>

namespace toeplitz = nygma::toeplitz;
namespace xoshiro = emptyspace::xoshiro;

union ipv4 {
  std::uint64_t _64;
  std::array<std::byte, 8> _array;
};

union ipv6 {
  std::uint64_t _256[4];
  std::array<std::byte, 32> _array;
};

int main() {
  xoshiro::xoshiro256starstar64 xo0{ 0x2342 };
  xoshiro::xoshiro256starstar64 xo1{ 0x2342 };
  xoshiro::xoshiro256starstar64 xo_warm{ 0x2342 };

  toeplitz::toeplitz_scalar_loop<toeplitz::rss_key_symmetric> rss_loop;
  toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_symmetric> rss_lut;

  emptyspace::pnch::config cfg;
  std::uint32_t h = 0;
  std::uint64_t x = 0;

  cfg.run( "xoshiro", [&]() { x ^= xo_warm(); } ).touch( x ).report_to( std::cerr );

  auto const offset = cfg.average();

  ipv4 v4;
  v4._64 = 0;

  cfg.run(
         "v4: toeplitz_scalar_loop",
         [&]() {
           v4._64 = xo0();
           h ^= rss_loop.hash<8>( v4._array.data() );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );

  cfg.run(
         "v4: toeplitz_scalar_lut",
         [&]() {
           v4._64 = xo1();
           h ^= rss_lut.hash<8>( v4._array.data() );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );

  ipv6 v6;
  v6._256[0] = 0;
  v6._256[1] = 0;
  v6._256[2] = 0;
  v6._256[3] = 0;

  xoshiro::xoshiro256starstar64 xo6_0{ 0x2342 };
  xoshiro::xoshiro256starstar64 xo6_1{ 0x2342 };

  cfg.run(
         "v6: toeplitz_scalar_loop",
         [&]() {
           v6._256[0] = xo6_0();
           v6._256[1] = xo6_0();
           v6._256[2] = xo6_0();
           v6._256[3] = xo6_0();
           h ^= rss_loop.hash<32>( v6._array.data() );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );

  cfg.run(
         "v6: toeplitz_scalar_lut",
         [&]() {
           v6._256[0] = xo6_1();
           v6._256[1] = xo6_1();
           v6._256[2] = xo6_1();
           v6._256[3] = xo6_1();
           h ^= rss_lut.hash<32>( v6._array.data() );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );

  return 0;
}
