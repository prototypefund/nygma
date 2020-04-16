// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/bytestring.hxx>
#include <libnygma/dissect.hxx>
#include <libnygma/dns.hxx>

namespace emptyspace::dns {

namespace unsafe = emptyspace::unsafe;

struct dns_trace : public dissect::dissect_trace {
  std::byte const* _src_begin;
  std::byte const* _src_end;
  std::byte const* _dst_begin;
  std::byte const* _dst_end;
  std::byte const* _begin;
  std::byte const* _end;
  std::byte const* _dns_begin;
  unsigned _assume_dns;

 public:
  template <typename V>
  inline void operator()( V&& v ) noexcept {
    using T = std::decay_t<decltype( v )>;
    constexpr auto BE = emptyspace::endianess::BE;
    if constexpr( std::is_same_v<T, dissect::ipv4> ) {
      std::byte const* const p = v._begin + 12;
      _src_begin = p;
      _src_end = p + 4;
      _dst_begin = p + 4;
      _dst_end = p + 8;
    } else if constexpr( std::is_same_v<T, dissect::ipv6> ) {
      std::byte const* const p = v._begin + 8;
      _src_begin = p;
      _src_end = p + 16;
      _dst_begin = p + 16;
      _dst_end = p + 32;
    } else if constexpr( std::is_same_v<T, dissect::udp> ) {
      _begin = v._begin;
      _end = v._end;
      std::byte const* const p = _begin;
      unsigned sp = unsafe::rd16<BE>( p );
      unsigned dp = unsafe::rd16<BE>( p + 2 );
      _assume_dns = sp == 53u || sp == 5353u || sp == 5355 || dp == 53u || dp == 5353u || dp == 5355u;
      _dns_begin = _begin + 8;
    } else if constexpr( std::is_same_v<T, dissect::tcp> ) {
      _begin = v._begin;
      _end = v._end;
      std::byte const* const p = _begin;
      unsigned len = unsigned( p[12] ) >> 2;
      unsigned sp = unsafe::rd16<BE>( p );
      unsigned dp = unsafe::rd16<BE>( p + 2 );
      _assume_dns = sp == 53u || sp == 5353u || sp == 5355 || dp == 53u || dp == 5353u || dp == 5355u;
      _dns_begin = _begin + len;
      _dns_begin += 2; // skip length prefix ( tcp only )
    }
  }

  inline bool valid() const noexcept { return _dns_begin < _end; }

  inline void rewind() noexcept { dissect::dissect_trace::rewind(); }
};

} // namespace emptyspace::dns
