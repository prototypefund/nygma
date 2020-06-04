// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libnygma/dissect.hxx>
#include <libriot/index-builder.hxx>
#include <libunclassified/bytestring.hxx>

namespace riot {

namespace unsafe = unclassified::unsafe;
namespace dissect = nygma::dissect;
using endianess = unclassified::endianess;

template <typename V4IndexType, typename PortIndexType>
struct index_trace : public nygma::dissect::dissect_trace {
  using v4_index_type = V4IndexType;
  using port_index_type = PortIndexType;

  static constexpr std::uint64_t SEGMENTSZ = ( 1ull << 32 ) - ( 4ull << 10 );

  std::uint64_t _v6_count{ 0 };
  std::uint64_t _v4_count{ 0 };
  std::uint64_t _udp_count{ 0 };
  std::uint64_t _tcp_count{ 0 };
  std::uint64_t _offset{ 0 };
  std::uint64_t _segment_offset{ 0 };
  std::size_t _count{ 0 };

  std::unique_ptr<v4_index_type> _v4_index;
  std::unique_ptr<port_index_type> _port_index;

 public:
  index_trace()
    : _v4_index{ std::make_unique<v4_index_type>() },
      _port_index{ std::make_unique<port_index_type>() } {}

  template <typename V>
  inline void operator()( V&& v ) noexcept {
    constexpr endianess BE = endianess::BE;
    using T = std::decay_t<decltype( v )>;
    if constexpr( std::is_same_v<T, dissect::ipv4> || std::is_same_v<T, dissect::ipv4f> ) {
      std::byte const* const p = v._begin + 12;
      auto* _src_begin = p;
      auto* _dst_begin = p + 4;
      auto _src_ip = unsafe::rd32<BE>( _src_begin );
      auto _dst_ip = unsafe::rd32<BE>( _dst_begin );
      _v4_index->add( _src_ip, static_cast<std::uint32_t>( _offset ) );
      _v4_index->add( _dst_ip, static_cast<std::uint32_t>( _offset ) );
      _v4_count++;
    } else if constexpr( std::is_same_v<T, dissect::ipv6> ) {
      //std::byte const* const p = v._begin + 8;
      //auto* _src_begin = p;
      //auto* _src_end = p + 16;
      //auto* _dst_begin = p + 16;
      //auto* _dst_end = p + 32;
      _v6_count++;
    } else if constexpr( std::is_same_v<T, dissect::udp> ) {
      std::byte const* const p = v._begin;
      auto _src_port = unsafe::rd16<BE>( p );
      auto _dst_port = unsafe::rd16<BE>( p + 2 );
      _port_index->add( _src_port, static_cast<std::uint32_t>( _offset ) );
      _port_index->add( _dst_port, static_cast<std::uint32_t>( _offset ) );
      _udp_count++;
    } else if constexpr( std::is_same_v<T, dissect::tcp> ) {
      std::byte const* const p = v._begin;
      auto _src_port = unsafe::rd16<BE>( p );
      auto _dst_port = unsafe::rd16<BE>( p + 2 );
      _port_index->add( _src_port, static_cast<std::uint32_t>( _offset ) );
      _port_index->add( _dst_port, static_cast<std::uint32_t>( _offset ) );
      _tcp_count++;
    }
  }

  template <typename Cycler>
  inline void prepare( std::uint64_t const offset, Cycler const c ) noexcept {
    if( offset - _segment_offset > SEGMENTSZ ) {
      c( std::move( _v4_index ), std::move( _port_index ), _segment_offset );
      _segment_offset = offset;
      _v4_index = std::make_unique<v4_index_type>();
      _port_index = std::make_unique<port_index_type>();
    }
    // store relative offset to the beginning of a segment @ `_segment_offset`
    _offset = offset - _segment_offset;
    dissect::dissect_trace::rewind();
  }

  template <typename Cycler>
  inline void finish( Cycler const c ) noexcept {
    // provide the last stored `_segment_offset` to the cycler
    c( std::move( _v4_index ), std::move( _port_index ), _segment_offset );
  }
};

} // namespace riot
