// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libnygma/dissect.hxx>

#include <array>
#include <cstdint>
#include <type_traits>

namespace dissect = nygma::dissect;

using hash_type = dissect::void_hash_policy;
using bytestring_view = unclassified::bytestring_view;
using dissect_tag = dissect::dissect_tag;

namespace {

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

// fragmented udp packet
/* Frame (70 bytes) */
static const unsigned char pkt_unsw_nb15[70] = {
0x00, 0x55, 0x22, 0xaf, 0xc6, 0x37, 0x00, 0x44, /* .U"..7.D */
0x66, 0xfc, 0x29, 0xaf, 0x08, 0x00, 0x45, 0x00, /* f.)...E. */
0x00, 0x38, 0x2b, 0x5c, 0x20, 0x00, 0xfe, 0x11, /* .8+\ ... */
0xfe, 0x6d, 0xaf, 0x2d, 0xb0, 0x00, 0x95, 0xab, /* .m.-.... */
0x7e, 0x11, 0x00, 0x35, 0x00, 0x35, 0x00, 0x24, /* ~..5.5.$ */
0x8c, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* .Q...... */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00              /* ...... */
};
// clang-format on

emptyspace::pest::suite basic( "dissect suite", []( auto& test ) {
  using namespace emptyspace::pest;
  test( "dissect pkt1", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt1 };
    auto const hash = dissect::dissect_en10mb( hash_policy, trace, bs );
    auto offset = [&]( unsigned const o ) { return ( bs.data() + o ); };

    expect( trace.index(), equal_to( 3u ) );
    expect( trace.entries()[0]._tag, equal_to( dissect_tag::eth ) );
    expect( trace.entries()[0]._data, equal_to( offset( 0 ) ) );
    expect( trace.entries()[1]._tag, equal_to( dissect_tag::ipv6 ) );
    expect( trace.entries()[1]._data, equal_to( offset( 14 ) ) );
    expect( trace.entries()[2]._tag, equal_to( dissect_tag::tcp ) );
    expect( trace.entries()[2]._data, equal_to( offset( 54 ) ) );
    expect( hash, equal_to( 0u ) );
  } );

  test( "dissect pkt2", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt2 };
    auto const hash = dissect::dissect_en10mb( hash_policy, trace, bs );
    auto offset = [&]( unsigned const o ) { return ( bs.data() + o ); };

    expect( trace.index(), equal_to( 3u ) );
    expect( trace.entries()[0]._tag, equal_to( dissect_tag::eth ) );
    expect( trace.entries()[0]._data, equal_to( offset( 0 ) ) );
    expect( trace.entries()[1]._tag, equal_to( dissect_tag::ipv4 ) );
    expect( trace.entries()[1]._data, equal_to( offset( 14 ) ) );
    expect( trace.entries()[2]._tag, equal_to( dissect_tag::tcp ) );
    expect( trace.entries()[2]._data, equal_to( offset( 34 ) ) );
    expect( hash, equal_to( 0u ) );
  } );

  test( "dissect pkt3", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt3 };
    auto const hash = dissect::dissect_en10mb( hash_policy, trace, bs );
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
  
  test( "dissect fragmented upd ( from unsw nb15 )", []( auto& expect ) {
    auto trace = dissect::dissect_stack_trace{};
    auto const hash_policy = hash_type{};
    auto const bs = bytestring_view{ pkt_unsw_nb15 };
    auto const hash = dissect::dissect_en10mb( hash_policy, trace, bs );
    auto offset = [&]( unsigned const o ) { return ( bs.data() + o ); };

    expect( trace.index(), equal_to( 3u ) );
    expect( trace.entries()[0]._tag, equal_to( dissect_tag::eth ) );
    expect( trace.entries()[0]._data, equal_to( offset( 0 ) ) );
    expect( trace.entries()[1]._tag, equal_to( dissect_tag::ipv4f ) );
    expect( trace.entries()[1]._data, equal_to( offset( 14 ) ) );
    expect( trace.entries()[2]._tag, equal_to( dissect_tag::udp ) );
    expect( trace.entries()[2]._data, equal_to( offset( 34 ) ) );
    expect( hash, equal_to( 0u ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
