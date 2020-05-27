// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pnch.hxx>
#include <pest/xoshiro.hxx>

#include <libnygma/dissect.hxx>
#include <libnygma/toeplitz.hxx>

#include <array>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace dissect = nygma::dissect;
namespace toeplitz = nygma::toeplitz;
namespace xoshiro = emptyspace::xoshiro;

using hash_type = toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_symmetric>;
using bytestring_view = unclassified::bytestring_view;
using dissect_tag = dissect::dissect_tag;

// clang-format off
 
/* Frame (86 bytes) */
static const unsigned char pkt1[86] = {
0x1c, 0x36, 0xbb, 0x13, 0x43, 0xe2, 0x3a, 0x17, /* .6..C.:. */
0xe1, 0xfb, 0xbb, 0xe0, 0x86, 0xdd, 0x64, 0x00, /* ......d. */
0x00, 0x00, 0x00, 0x20, 0x06, 0x3a, 0x20, 0x01, /* ... .: . */
0x05, 0x58, 0xfe, 0xed, 0x00, 0x00, 0x00, 0x00, /* .X...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x26, 0x03, /* ......&. */
0x30, 0x01, 0x19, 0x75, 0xd0, 0x00, 0x3d, 0xa3, /* 0..u..=. */
0xc2, 0xea, 0x4a, 0x00, 0x2c, 0xcc, 0x00, 0x35, /* ..J.,..5 */
0xec, 0xc0, 0xdd, 0x6b, 0x34, 0x5e, 0xe6, 0x00, /* ...k4^.. */
0xe2, 0x32, 0x80, 0x10, 0x00, 0x10, 0xfb, 0x50, /* .2.....P */
0x00, 0x00, 0x01, 0x01, 0x08, 0x0a, 0x1b, 0x4e, /* .......N */
0x4f, 0x91, 0x3c, 0x1e, 0x32, 0x4f              /* O.<.2O */
};

/* Frame (78 bytes) */
static const unsigned char pkt2[78] = {
0x88, 0xb1, 0xe1, 0xd4, 0x88, 0x31, 0x1c, 0x36, /* .....1.6 */
0xbb, 0x13, 0x43, 0xe2, 0x08, 0x00, 0x45, 0x00, /* ..C...E. */
0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x40, 0x06, /* .@..@.@. */
0x19, 0x2a, 0xac, 0x10, 0x65, 0x6e, 0x08, 0x08, /* .*..en.. */
0x08, 0x08, 0xcf, 0x63, 0x00, 0x35, 0xa7, 0xd0, /* ...c.5.. */
0x4b, 0x8c, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x02, /* K....... */
0xff, 0xff, 0x92, 0xfe, 0x00, 0x00, 0x02, 0x04, /* ........ */
0x05, 0xb4, 0x01, 0x03, 0x03, 0x06, 0x01, 0x01, /* ........ */
0x08, 0x0a, 0x3b, 0xd5, 0x83, 0xa4, 0x00, 0x00, /* ..;..... */
0x00, 0x00, 0x04, 0x02, 0x00, 0x00              /* ...... */
};

/* Frame (64 bytes) */
// arp / qiniq (double 0x8100)
static const unsigned char pkt3[64] = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xca, 0x03, /* ........ */
0x0d, 0xb4, 0x00, 0x1c, 0x81, 0x00, 0x00, 0x64, /* .......d */
0x81, 0x00, 0x00, 0xc8, 0x08, 0x06, 0x00, 0x01, /* ........ */
0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0xca, 0x03, /* ........ */
0x0d, 0xb4, 0x00, 0x1c, 0xc0, 0xa8, 0x02, 0xc8, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, /* ........ */
0x02, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /* ........ */
};

// clang-format on

int main() {
  emptyspace::pnch::config cfg;
  std::uint32_t h = 0;

  xoshiro::xoshiro256starstar64 xo0{ 0x2342 };
  xoshiro::xoshiro256starstar64 xo1{ 0x2342 };
  xoshiro::xoshiro256starstar64 xo_warm{ 0x2342 };

  cfg.run( "xoshiro", [&]() { h ^= xo_warm(); } ).touch( h ).report_to( std::cerr );

  auto const offset = cfg.average();

  auto trace = dissect::dissect_stack_trace{};
  auto const hash_policy = hash_type{};
  auto const bs1 = bytestring_view{ pkt1 };
  auto const bs2 = bytestring_view{ pkt2 };
  auto const bs3 = bytestring_view{ pkt3 };

  constexpr int N = 4;
  static_assert( is_power_of_two_v<N> );
  std::array<bytestring_view const, N> mix{ bs1, bs2, bs3, bs3 };

  cfg.run(
         "dissect pkt1 (scalar)",
         [&]() {
           trace.rewind();
           h ^= dissect::dissect_en10mb( hash_policy, trace, bs1 );
         } )
      .touch( h )
      .report_to( std::cerr );

  cfg.run(
         "dissect pkt2 (scalar)",
         [&]() {
           trace.rewind();
           h ^= dissect::dissect_en10mb( hash_policy, trace, bs2 );
         } )
      .touch( h )
      .report_to( std::cerr );

  cfg.run(
         "dissect mix (scalar)",
         [&]() {
           trace.rewind();
           auto const& bs = mix[xo1() & ( N - 1 )];
           h ^= dissect::dissect_en10mb( hash_policy, trace, bs );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );

  /*
  cfg.run(
         "dissect pkt2 (avx2)",
         [&]() {
           trace.rewind();
           h ^= dissect::dissect_en10mb_avx2( hash_policy, trace, bs2 );
         } )
      .touch( h )
      .report_to( std::cerr );

  cfg.run(
         "dissect mix (avx2)",
         [&]() {
           trace.rewind();
           auto const& bs = mix[xo0() & ( N - 1 )];
           h ^= dissect::dissect_en10mb_avx2( hash_policy, trace, bs );
         } )
      .touch( h )
      .report_to( std::cerr )
      .offset( offset )
      .report_to( std::cerr, "normalized" );
  */
  return 0;
}
