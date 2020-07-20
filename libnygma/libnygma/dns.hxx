// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

// uses enums based on ( with modifications )
//   - <https://github.com/robertdavidgraham/dnsparse/blob/master/src/dns-parse.h>
//   - <https://github.com/farsightsec/wdns/blob/master/wdns/wdns.h.in>

#include <libnygma/dissect.hxx>
#include <libunclassified/bytestring.hxx>

#include <cstring>
#include <string_view>

namespace nygma::dns {
namespace detail {

namespace unsafe = unclassified::unsafe;
using endianess = unclassified::endianess;

inline std::uint16_t rd16( unsigned char const* p ) noexcept {
  return unsafe::rd16<endianess::BE>( reinterpret_cast<std::byte const*>( p ) );
}

inline std::uint32_t rd32( unsigned char const* p ) noexcept {
  return unsafe::rd32<endianess::BE>( reinterpret_cast<std::byte const*>( p ) );
}

} // namespace detail

enum class dns_dissect_rc {
  OK,
  QNAME_OVERFLOW,
  RNAME_OVERFLOW,
  HEADER_UNDERFLOW,
  RDATA_OVERFLOW,
  RECORD_OVERFLOW,
};

inline std::string_view const to_string( dns_dissect_rc const code ) noexcept {
  using rc = dns_dissect_rc;
  switch( code ) {
    case rc::OK: return "OK";
    case rc::QNAME_OVERFLOW: return "QNAME_OVERFLOW";
    case rc::RNAME_OVERFLOW: return "RNAME_OVERFLOW";
    case rc::HEADER_UNDERFLOW: return "HEADER_UNDERFLOW";
    case rc::RDATA_OVERFLOW: return "RDATE_OVERFLOW";
    case rc::RECORD_OVERFLOW: return "RECORD_OVERFLOW";
  }
}

struct dns_op {
  using type = unsigned;
  enum : type {
    QUERY = 0u,
    IQUERY = 1u,
    STATUS = 2u,
    NOTIFY = 4u,
    UPDATE = 5u,
  };
};

struct dns_rcode {
  using type = unsigned;
  enum : type {
    NOERROR = 0,
    FORMERR = 1, // Format Error                     [RFC1035]
    SERVFAIL = 2, // Server Failure                   [RFC1035]
    NXDOMAIN = 3, // Non-Existent Domain              [RFC1035]
    NOTIMP = 4, // Not Implemented                  [RFC1035]
    REFUSED = 5, // Query Refused                    [RFC1035]
    YXDOMAIN = 6, // Name Exists when it should't     [RFC2136]
    YXRRSET = 7, // RR Set Exists when it should't   [RFC2136]
    NXRRSET = 8, // RR Set that should exist does't  [RFC2136]
    NOTAUTH = 9, // Not Authorized                   [RFC2845]
    NOTZONE = 10, // Name not contained in zone       [RFC2136]
    BADSIG = 16, // TSIG Signature Failure           [RFC2845]
    BADKEY = 17, // Key not recognized               [RFC2845]
    BADTIME = 18, // Signature out of time window     [RFC2845]
    BADMODE = 19, // Bad TKEY Mode                    [RFC2930]
    BADNAME = 20, // Duplicate key name               [RFC2930]
    BADALG = 21, // Algorithm not supported          [RFC2930]
    BADTRUNC = 22, // Bad Truncation                   [RFC4635]
  };
};

struct dns_class {
  using type = unsigned;
  enum : type {
    IN = 1,
    CH = 3,
    HS = 4,
    NONE = 254,
    ANY = 255,
  };
};

struct dns_type {
  using type = unsigned;
  enum : type {
    A = 1,
    NS = 2,
    MD = 3,
    MF = 4,
    CNAME = 5,
    SOA = 6,
    MB = 7,
    MG = 8,
    MR = 9,
    NUL = 10,
    WKS = 11,
    PTR = 12,
    HINFO = 13,
    MINFO = 14,
    MX = 15,
    TXT = 16,
    RP = 17,
    AFSDB = 18,
    X25 = 19,
    ISDN = 20,
    RT = 21,
    NSAP = 22,
    NSAP_PTR = 23,
    SIG = 24,
    KEY = 25,
    PX = 26,
    GPOS = 27,
    AAAA = 28,
    LOC = 29,
    NXT = 30,
    EID = 31,
    NIMLOC = 32,
    SRV = 33,
    ATMA = 34,
    NAPTR = 35,
    KX = 36,
    CERT = 37,
    A6 = 38,
    DNAME = 39,
    SINK = 40,
    OPT = 41,
    APL = 42,
    DS = 43,
    SSHFP = 44,
    IPSECKEY = 45,
    RRSIG = 46,
    NSEC = 47,
    DNSKEY = 48,
    DHCID = 49,
    NSEC3 = 50,
    NSEC3PARAM = 51,
    TLSA = 52,
    // unassigned: 53 - 54
    HIP = 55,
    NINFO = 56,
    RKEY = 57,
    TALINK = 58,
    CDS = 59,
    CDNSKEY = 60,
    OPENPGPKEY = 61,
    CSYNC = 62,
    // unassigned: 63 - 98
    SPF = 99,
    UINFO = 100,
    UID = 101,
    GID = 102,
    UNSPEC = 103,
    NID = 104,
    L32 = 105,
    L64 = 106,
    LP = 107,
    EUI48 = 108,
    EUI64 = 109,
    // unassigned: 110 - 248
    TKEY = 249,
    TSIG = 250,
    IXFR = 251,
    AXFR = 252,
    MAILB = 253,
    MAILA = 254,
    ANY = 255,
    URI = 256,
    CAA = 257,
    // unassigned: 258 - 32767
    TA = 32768,
    DLV = 32769,
    // unassigned: 32770 - 65279
    // private use: 65280 - 65534
    // reserved: 65535
  };
};

template <std::size_t LabelSize>
struct dns_rr {
  unsigned char const* _rr_begin;
  unsigned char const* _rr_link;

  unsigned _is_compressed : 1;
  unsigned _is_loop_or_fwd : 1;
  unsigned _is_x : 1;
  unsigned _is_edns0 : 1; // edns0 opt rr found
  struct {
    unsigned _cache_flush : 1;
    unsigned _do : 1;
    unsigned _udp_size;
    unsigned _version;
    unsigned _z;
    unsigned _rcode_ex;
  } _edns0;

  unsigned short _type;
  unsigned short _class;
  unsigned _ttl;

  unsigned _rdata_len;
  std::byte const* _rdata_begin;

  unsigned _label_len;
  unsigned char _label[LabelSize];

  constexpr std::string_view label() const noexcept {
    return { reinterpret_cast<char const*>( _label ), _label_len };
  }

  constexpr std::string_view data() const noexcept {
    return { reinterpret_cast<char const*>( _rdata_begin ), _rdata_len };
  }
};

template <std::size_t RecordCnt, std::size_t LabelSize>
struct dns {
  using dns_rr_t = dns_rr<LabelSize>;

  static constexpr auto MAX_RECORD_COUNT = RecordCnt;

  unsigned _is_header_finished : 1;
  unsigned _is_packet_finished : 1;
  unsigned _qr : 1; // 'query' or 'response'
  unsigned _aa : 1; // 'authoritative answer'
  unsigned _tc : 1; // 'truncation'
  unsigned _rd : 1; // 'recursion desired'
  unsigned _ra : 1; // 'recursion available'
  unsigned _z : 1; // reserved
  unsigned _ad : 1;
  unsigned _cd : 1;
  unsigned _id; // transaction id
  unsigned _opcode;
  unsigned _rcode; // response error code
  unsigned _qdcount; // query count
  unsigned _ancount; // answer count
  unsigned _nscount; // name-server/authority count
  unsigned _arcount; // additional record count
  unsigned char const* _dns_begin;
  unsigned char const* _dns_end;
  std::byte const* _src_begin;
  std::byte const* _src_end;
  std::byte const* _dst_begin;
  std::byte const* _dst_end;
  unsigned _req_length;
  unsigned _rr_count;
  dns_rr_t _rr[MAX_RECORD_COUNT];
  std::byte _timestamp[sizeof( std::uint64_t )];

  inline unsigned char const* extract_name( unsigned char const* const begin,
                                            unsigned char const* const end, dns_rr_t& rr ) {
    unsigned char const* p = begin;
    unsigned char* label_out = rr._label;
    unsigned char* label_end = rr._label + LabelSize;
    unsigned char const* link = nullptr;
    unsigned char const* guard = rr._rr_begin;
    rr._is_compressed = 0;
    rr._label_len = 0;
    rr._is_x = 0;
    while( p < end ) {
      unsigned len = *p++;
      if( ( len & 0b1100'0000 ) == 0b1100'0000 ) {
        if( ( p + 1 ) > end ) {
          rr._label[0] = '\0';
          rr._label_len = 0;
          return end;
        }

        rr._is_compressed = 1;
        link = link == nullptr ? ( p + 1 ) : link;
        guard = p - 1;
        unsigned offset = ( len & 0x3f ) << 8 | unsigned( *p );
        p = _dns_begin + offset;
        // only allow backward links
        if( p >= guard ) {
          rr._label[0] = '\0';
          rr._label_len = 0;
          rr._is_loop_or_fwd = 1;
          return end;
        }
      } else if( len & 0b1100'0000 ) {
        rr._label[0] = '\0';
        rr._label_len = 0;
        rr._is_x = 1;
        return end;
      } else {
        if( len == 0 ) {
          if( rr._is_compressed ) { return link; }
          return p;
        } else {
          if( ( label_out + len + 1 ) > label_end or ( p + len ) > end ) {
            rr._label[0] = '\0';
            rr._label_len = 0;
            return end;
          }
          std::memcpy( label_out, p, len );
          label_out[len] = '.';
          rr._label_len += len + 1;
          label_out += len + 1;
          p += len;
        }
      }
    }
    // only reached if `begin >= end`
    rr._label[0] = '\0';
    rr._label_len = 0;
    return end;
  }

  inline dns_dissect_rc dissect( std::byte const* const begin, std::byte const* const _end ) noexcept {
    unsigned char const* p = reinterpret_cast<unsigned char const*>( begin );
    unsigned char const* end = reinterpret_cast<unsigned char const*>( _end );

    _is_header_finished = 0;
    _is_packet_finished = 0;

    _dns_begin = p;

    /*
     rfc 2929
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    if( p + 12 > end ) { return dns_dissect_rc::HEADER_UNDERFLOW; }

    _id = detail::rd16( p );
    _qr = ( p[2] >> 7 ) & 1;
    _opcode = ( p[2] >> 3 ) & 0xf;
    _aa = ( p[2] >> 2 ) & 1;
    _tc = ( p[2] >> 1 ) & 1;
    _rd = p[2] & 1;
    _ra = ( p[3] >> 7 ) & 1;
    _z = ( p[3] >> 6 ) & 1;
    _ad = ( p[3] >> 5 ) & 1;
    _cd = ( p[3] >> 4 ) & 1;
    _rcode = p[3] & 0xf;
    _qdcount = detail::rd16( p + 4 );
    _ancount = detail::rd16( p + 6 );
    _nscount = detail::rd16( p + 8 );
    _arcount = detail::rd16( p + 10 );

    _is_header_finished = 1;

    p += 12;
    _rr_count = 0;

    /*
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */

    for( unsigned i = 0; i < _qdcount && p < end; i++ ) {
      if( _rr_count >= MAX_RECORD_COUNT ) { return dns_dissect_rc::RECORD_OVERFLOW; }
      auto& rr = _rr[_rr_count];
      rr._is_edns0 = 0;
      rr._rr_begin = p;
      rr._rdata_len = 0;
      rr._rdata_begin = nullptr;
      rr._ttl = 0;
      p = extract_name( p, end, rr );
      if( p + 4 > end ) { return dns_dissect_rc::QNAME_OVERFLOW; }
      rr._type = detail::rd16( p );
      rr._class = detail::rd16( p + 2 );
      // no rdata for queries, so just continue
      _rr_count++;
      p += 4;
    }

    /*
                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    for( unsigned i = 0; i < _ancount + _nscount && p < end; i++ ) {
      if( _rr_count >= MAX_RECORD_COUNT ) { return dns_dissect_rc::RECORD_OVERFLOW; }
      auto& rr = _rr[_rr_count];
      rr._is_edns0 = 0;
      rr._rr_begin = p;
      p = extract_name( p, end, rr );
      if( p + 10 > end ) { return dns_dissect_rc::RNAME_OVERFLOW; }
      rr._type = detail::rd16( p );
      rr._class = detail::rd16( p + 2 );
      rr._ttl = detail::rd32( p + 4 );
      rr._rdata_len = detail::rd16( p + 8 );
      rr._rdata_begin = reinterpret_cast<std::byte const*>( p + 10 );
      p += rr._rdata_len + 10;
      if( p > end ) { return dns_dissect_rc::RDATA_OVERFLOW; }
      _rr_count++;
    }

    for( unsigned i = 0; i < _arcount && p < end; i++ ) {
      if( _rr_count >= MAX_RECORD_COUNT ) { return dns_dissect_rc::RECORD_OVERFLOW; }
      auto& rr = _rr[_rr_count];
      rr._is_edns0 = 0;
      rr._rr_begin = p;
      // edns0 opt rr: rfc 2671
      if( ( p + 9 ) < end && p[0] == 0u && p[1] == 0u && p[2] == 41u ) {
        // this overlaps with xclass = detail::rd16(p+2) of
        // normal rr's.
        rr._is_edns0 = 1;
        auto clazz = detail::rd16( p + 3 );
        auto ttl = detail::rd32( p + 5 );
        rr._edns0._do = ( ttl >> 15 ) & 1;
        rr._edns0._rcode_ex = ( ttl > 24 ) & 0xff;
        rr._edns0._version = ( ttl >> 16 ) & 0xff;
        rr._edns0._z = ttl & 0x7fff;
        rr._edns0._udp_size = clazz & 0b0111'1111'1111'1111;
        rr._edns0._cache_flush = ( clazz >> 31 ) & 1;
      }
      p = extract_name( p, end, rr );
      if( p + 10 > end ) { return dns_dissect_rc::RNAME_OVERFLOW; }
      rr._type = detail::rd16( p );
      rr._class = detail::rd16( p + 2 );
      rr._ttl = detail::rd32( p + 4 );
      rr._rdata_len = detail::rd16( p + 8 );
      rr._rdata_begin = reinterpret_cast<std::byte const*>( p + 10 );
      p += rr._rdata_len + 10;
      if( p > end ) { return dns_dissect_rc::RDATA_OVERFLOW; }
      _rr_count++;
    }

    _is_packet_finished = p <= end;
    _dns_end = p;

    return dns_dissect_rc::OK;
  }
};

using dns_t = dns<64, 1024>;

} // namespace nygma::dns
