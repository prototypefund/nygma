// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/support.hxx>
#include <libunclassified/bytestring.hxx>

#include <array>
#include <iostream>

#include <immintrin.h>

namespace nygma::dissect {

namespace unsafe = unclassified::unsafe;
using endianess = unclassified::endianess;
using bytestring_view = unclassified::bytestring_view;

enum class dissect_tag {
  arp,
  ectp,
  eth,
  icmpv4,
  icmpv6,
  ipv4,
  ipv4f,
  ipv6,
  ipv6f,
  llc,
  lldp,
  mpls,
  sctp,
  tcp,
  udp,
  vlan_8021q,
  unkown,
};

class dissect_trace {
  static constexpr unsigned int ENTRIES = 64;
  static constexpr unsigned int MASK = ENTRIES - 1;

  static_assert( is_power_of_two_v<ENTRIES> );

 public:
  struct entry {
    dissect_tag _tag;
    std::byte const* _data;
  };

 private:
  std::array<entry, ENTRIES> _stack;
  unsigned int _idx;

 public:
  dissect_trace() {
    for( unsigned i = 0; i < _stack.size(); i++ ) {
      _stack[i]._tag = dissect_tag::unkown;
      _stack[i]._data = nullptr;
    }
    _idx = 0;
  }

  inline void rewind() noexcept { _idx = 0; }

  inline auto index() const noexcept { return _idx; }

  inline auto const& entries() const noexcept { return _stack; }

  inline auto const& operator[]( unsigned const index ) noexcept { return _stack[index]; }

  inline void push( dissect_tag const tag, std::byte const* const p ) noexcept {
    _stack[_idx & MASK]._tag = tag;
    _stack[_idx & MASK]._data = p;
    _idx++;
  }
};

struct entity {
 public:
  std::byte const* const _begin;
  std::byte const* const _end;
};

struct arp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::arp;
};
struct eth final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::eth;
};
struct icmpv4 final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::icmpv4;
};
struct icmpv6 final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::icmpv6;
};
struct ipv4 final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::ipv4;
};
struct ipv4f final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::ipv4f;
};
struct ipv6 final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::ipv6;
};
struct ipv6f final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::ipv6f;
};
struct tcp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::tcp;
};
struct udp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::udp;
};
struct unkown final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::unkown;
};
struct vlan_8021q final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::vlan_8021q;
};
struct sctp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::sctp;
};
struct mpls final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::mpls;
};
struct llc final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::llc;
};
struct lldp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::lldp;
};
struct ectp final : public entity {
  static constexpr dissect_tag _tag = dissect_tag::ectp;
};

class dissect_stack_trace : public dissect::dissect_trace {
 public:
  template <typename V>
  inline void operator()( V&& v ) noexcept {
    push( v._tag, v._begin );
  }
};

#define _next0_( allow_vlan, x )                                                                      \
  do {                                                                                                \
    if constexpr( allow_vlan ) {                                                                      \
      switch( x ) {                                                                                   \
        case 0x0800: goto parse_ipv4;                                                                 \
        case 0x0806: goto parse_arp;                                                                  \
        case 0x86dd: goto parse_ipv6;                                                                 \
        case 0x8100: goto parse_vlan_8021q;                                                           \
        case 0x8a88: goto parse_vlan_8021q;                                                           \
        case 0x9100: goto parse_vlan_8021q;                                                           \
        case 0x8847: goto parse_vlan_mpls;                                                            \
        case 0x88CC: goto parse_lldp;                                                                 \
        case 0x9000: goto parse_ectp;                                                                 \
        default: trace.push( dissect_tag::unkown, p ); return hash;                                   \
      }                                                                                               \
    } else {                                                                                          \
      switch( x ) {                                                                                   \
        case 0x0800: goto parse_ipv4;                                                                 \
        case 0x0806: goto parse_arp;                                                                  \
        case 0x86dd: goto parse_ipv6;                                                                 \
        case 0x8847: goto parse_vlan_mpls;                                                            \
        case 0x88CC: goto parse_lldp;                                                                 \
        case 0x9000: goto parse_ectp;                                                                 \
        default: trace.push( dissect_tag::unkown, p ); return hash;                                   \
      }                                                                                               \
    }                                                                                                 \
  } while( false )

struct void_hash_policy {
  template <std::size_t N>
  inline std::uint32_t hash( std::byte const* const ) const noexcept {
    return 0;
  }
};

template <typename HashPolicy, typename Trace, bool Cont = true>
static inline std::uint32_t dissect_en10mb(
    HashPolicy& hash_policy, Trace&& trace, bytestring_view const& view ) noexcept {
  constexpr endianess BE = endianess::BE;

  std::byte const* const begin = view.data();
  std::byte const* const end = begin + view.size();
  std::byte const* p = begin;
  std::uint32_t hash = 0;

  if( view.size() < 20 ) { return 0u; }

  trace( eth{ { p, end } } );
  auto ethertype = unsafe::rd16<BE>( p + 12 );
  auto const vlan_ex = unsafe::rd16<BE>( p + 16 );
  auto const vlan_et = unsafe::rd16<BE>( p + 20 );
  p += 14;

  _next0_( true, ethertype );

parse_vlan_8021q:
  trace( vlan_8021q{ { p, end } } );
  p += vlan_ex == 0x8100u ? 8 : 4;
  ethertype = vlan_ex == 0x8100u ? vlan_et : vlan_ex;
  _next0_( false, ethertype );

parse_ipv4 : {
  if( p >= end ) {
    trace( unkown{ { p, end } } );
    return hash;
  }
  unsigned const x = static_cast<unsigned>( *p );
  unsigned const len = ( x & 0x0f ) << 2;
  if( p + len > end ) {
    trace( unkown{ { p, end } } );
    return hash;
  }
  unsigned const part = unsafe::rd16<endianess::BE>( p + 6 );
  //unsigned const flag = part & 0b0010'0000'0000'0000u;
  //unsigned const foffset = part & 0b0001'1111'1111'1111u;
  hash = hash_policy.template hash<8>( p + 12 );
  if( part & 0b0011'1111'1111'1111u ) {
    trace( ipv4f{ { p, end } } );
    if( auto const foffset = part & 0b0001'1111'1111'1111u; foffset != 0 ) { return hash; }
  } else {
    trace( ipv4{ { p, end } } );
  }
  if constexpr( Cont ) {
    unsigned const transport = static_cast<unsigned>( p[9] );
    p += len;
    switch( transport ) {
      case 1: goto parse_icmpv4;
      case 6: goto parse_tcp;
      case 17: goto parse_udp;
      case 132: goto parse_sctp;
      default: return hash;
    }
  }
  return hash;
}

parse_tcp:
  trace( tcp{ { p, end } } );
  return hash;

parse_udp:
  trace( udp{ { p, end } } );
  return hash;

parse_icmpv4:
  trace( icmpv4{ { p, end } } );
  return hash;

parse_icmpv6:
  trace( icmpv6{ { p, end } } );
  return hash;

parse_sctp:
  trace( unkown{ { p, end } } );
  return hash;

parse_lldp:
  trace( unkown{ { p, end } } );
  return hash;

parse_ectp:
  trace( unkown{ { p, end } } );
  return hash;

parse_arp:
  trace( arp{ { p, end } } );
  return hash;

parse_ipv6 : {
  if( p + 40 > end ) {
    trace( unkown{ { p, end } } );
    return hash;
  }
  hash = hash_policy.template hash<32>( p + 8 );
  unsigned const len = unsafe::rd16<BE>( p + 4 );
  if( p + len + 40 > end ) {
    trace( unkown{ { p, end } } );
    return hash;
  }
  trace( ipv6{ { p, end } } );
  if constexpr( Cont ) {
    unsigned const transport = static_cast<unsigned>( p[6] );
    p += 40;
    switch( transport ) {
      case 6: goto parse_tcp;
      case 17: goto parse_udp;
      case 58: goto parse_icmpv6;
      default: return hash;
    }
  }
  return hash;
}

parse_vlan_mpls:
  trace( unkown{ { p, end } } );
  return hash;
}

#undef _next0_

} // namespace nygma::dissect
