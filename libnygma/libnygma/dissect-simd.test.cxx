// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/dissect.hxx>
#include <libnygma/dissect-simd.hxx>

#include <array>
#include <cstdint>
#include <type_traits>

namespace dissect = emptyspace::dissect;

using hash_type = dissect::void_hash_policy;
using bytestring_view = emptyspace::bytestring_view;
using dissect_tag = dissect::dissect_tag;

namespace {

// clang-format off

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

emptyspace::pest::suite basic( "dissect suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "avx2: dissect pkt2", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt2 };
    auto const hash = dissect::dissect_en10mb_avx2( hash_policy, trace, bs );
    auto offset = [&]( unsigned const o ) { return ( bs.data() + o ); };

    expect( trace.index(), equal_to( 2u ) );
    expect( trace.entries()[0]._tag, equal_to( dissect_tag::eth ) );
    expect( trace.entries()[0]._data, equal_to( offset( 0 ) ) );
    expect( trace.entries()[1]._tag, equal_to( dissect_tag::ipv4 ) );
    expect( trace.entries()[1]._data, equal_to( offset( 14 ) ) );
    expect( hash, equal_to( 0u ) );
  } );

  test( "avx2: dissect pkt3", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt3 };
    auto const hash = dissect::dissect_en10mb_avx2( hash_policy, trace, bs );
    auto offset = [&]( unsigned const o ) { return ( bs.data() + o ); };

    expect( trace.index(), equal_to( 3u ) );
    expect( trace.entries()[0]._tag, equal_to( dissect_tag::eth ) );
    expect( trace.entries()[0]._data, equal_to( offset( 0 ) ) );
    expect( trace.entries()[1]._tag, equal_to( dissect_tag::vlan_8021q ) );
    expect( trace.entries()[1]._data, equal_to( offset( 14 ) ) );
    expect( trace.entries()[2]._tag, equal_to( dissect_tag::arp ) );
    expect( trace.entries()[2]._data, equal_to( offset( 22 ) ) );
    expect( hash, equal_to( 0u ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
