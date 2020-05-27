// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libnygma/dissect.hxx>
#include <libnygma/support.hxx>

#include <libunclassified/bytestring.hxx>

#include <array>
#include <iostream>

#include <immintrin.h>

namespace nygma::dissect {

namespace unsafe = unclassified::unsafe;
using endianess = unclassified::endianess;
using bytestring_view = unclassified::bytestring_view;

#define _next_( allow_vlan, x )                                                                       \
  do {                                                                                                \
    if constexpr( allow_vlan ) {                                                                      \
      switch( x ) {                                                                                   \
        case be16<0x0800>(): goto parse_ipv4;                                                         \
        case be16<0x0806>(): goto parse_arp;                                                          \
        case be16<0x86dd>(): goto parse_ipv6;                                                         \
        case be16<0x8100>(): goto parse_vlan_8021q;                                                   \
        case be16<0x8a88>(): goto parse_vlan_8021q;                                                   \
        case be16<0x9100>(): goto parse_vlan_8021q;                                                   \
        case be16<0x8847>(): goto parse_vlan_mpls;                                                    \
        case be16<0x88CC>(): goto parse_lldp;                                                         \
        case be16<0x9000>(): goto parse_ectp;                                                         \
        default: trace.push( dissect_tag::unkown, p ); return hash;                                   \
      }                                                                                               \
    } else {                                                                                          \
      switch( x ) {                                                                                   \
        case be16<0x0800>(): goto parse_ipv4;                                                         \
        case be16<0x0806>(): goto parse_arp;                                                          \
        case be16<0x86dd>(): goto parse_ipv6;                                                         \
        case be16<0x8847>(): goto parse_vlan_mpls;                                                    \
        case be16<0x88CC>(): goto parse_lldp;                                                         \
        case be16<0x9000>(): goto parse_ectp;                                                         \
        default: trace.push( dissect_tag::unkown, p ); return hash;                                   \
      }                                                                                               \
    }                                                                                                 \
  } while( false )

// unfinished: do not use!
template <typename Trace, typename HashPolicy>
static inline std::uint32_t dissect_en10mb_avx2(
    HashPolicy& hash_policy, Trace& trace, bytestring_view const& view ) noexcept {
  std::byte const* const begin = view.data();
  std::byte const* const end = begin + view.size();
  std::byte const* p = begin;
  std::uint32_t hash = 0;

  if( view.size() < 60 ) return 0u;

  __m256i const* pp = reinterpret_cast<__m256i const*>( p + 12 );
  __m256i x = _mm256_loadu_si256( pp );

  trace.push( dissect_tag::eth, p );
  p += 14;
  auto ethertype = _mm256_extract_epi16( x, 0 );
  auto const vlan_ex = _mm256_extract_epi16( x, 2 );
  auto const vlan_et = _mm256_extract_epi16( x, 4 );

  _next_( true, ethertype );

parse_vlan_8021q:
  trace.push( dissect_tag::vlan_8021q, p );
  p += vlan_ex == be16<0x8100u>() ? 8 : 4;
  ethertype = vlan_ex == be16<0x8100u>() ? vlan_et : vlan_ex;
  _next_( false, ethertype );

parse_ipv4:
  if( p + 20 > end ) {
    trace.push( dissect_tag::unkown, p );
    return hash;
  }
  trace.push( dissect_tag::ipv4, p );
  hash = hash_policy.template hash<8>( p + 12 );
  return hash;

parse_ipv6:
  if( p + 40 > end ) {
    trace.push( dissect_tag::unkown, p );
    return hash;
  }
  trace.push( dissect_tag::ipv6, p );
  hash = hash_policy.template hash<32>( p + 8 );
  return hash;

parse_lldp:
  trace.push( dissect_tag::lldp, p );
  return hash;

parse_ectp:
  trace.push( dissect_tag::llc, p );
  return hash;

parse_arp:
  trace.push( dissect_tag::arp, p );
  return hash;

parse_vlan_mpls:
  trace.push( dissect_tag::mpls, p );
  return hash;
}

#undef _next_

} // namespace nygma::dissect
