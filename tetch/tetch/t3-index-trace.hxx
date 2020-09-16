// License: RoundRobin-1.0.0

#pragma once

#include <libnygma/dissect.hxx>
#include <libnygma/dns.hxx>
#include <libriot/index-builder.hxx>
#include <libunclassified/bytestring.hxx>
#include <libunclassified/femtolog.hxx>

namespace t3tch {

namespace unsafe = unclassified::unsafe;
namespace dissect = nygma::dissect;
using endianess = unclassified::endianess;

template <typename IndexType, typename Engine>
struct index_trace : public nygma::dissect::dissect_trace {
  using index_type = IndexType;
  using engine_type = Engine;
  static constexpr std::uint64_t SEGMENTSZ = ( 1ull << 32 ) - ( 4ull << 10 );

  std::uint64_t _v6_count{ 0 };
  std::uint64_t _v4_count{ 0 };
  std::uint64_t _udp_count{ 0 };
  std::uint64_t _tcp_count{ 0 };
  std::uint64_t _dns_count{ 0 };
  std::uint64_t _offset{ 0 };
  std::uint64_t _segment_offset{ 0 };
  std::size_t _count{ 0 };

  engine_type _engine;
  engine_type _engine_dns;
  nygma::dns::dns_t _dns;
  std::byte const* _labels[decltype( _dns )::MAX_RECORD_COUNT];
  unsigned int _labels_length[decltype( _dns )::MAX_RECORD_COUNT];

  std::unique_ptr<index_type> _index;
  std::vector<unsigned> _matched_ids;

 public:
  index_trace( Engine&& engine, Engine&& engine_dns )
    : _engine{ std::forward<engine_type>( engine ) },
      _engine_dns{ std::forward<engine_type>( engine_dns ) },
      _index{ std::make_unique<index_type>() } {
    for( std::size_t i = 0; i < decltype( _dns )::MAX_RECORD_COUNT; i++ ) {
      _labels[i] = reinterpret_cast<std::byte const*>( &_dns._rr[i]._label[0] );
    }
  }

  template <typename V>
  inline void operator()( V&& v ) noexcept { //constexpr endianess BE = endianess::BE;
    using T = std::decay_t<decltype( v )>;
    constexpr auto BE = endianess::BE;

    if constexpr( std::is_same_v<T, dissect::ipv4> || std::is_same_v<T, dissect::ipv4f> ) {
      _v4_count++;

    } else if constexpr( std::is_same_v<T, dissect::ipv6> ) {
      _v6_count++;

      // TODO: refactor udp and tcp matching

    } else if constexpr( std::is_same_v<T, dissect::udp> ) {
      std::byte const* const payload = v._begin + 8;
      auto payload_size = static_cast<std::size_t>( v._end - payload );
      auto sp = unsafe::rd16<BE>( v._begin );
      auto dp = unsafe::rd16<BE>( v._begin + 2 );
      if( sp == 53u || sp == 5353u || sp == 5355 || dp == 53u || dp == 5353u || dp == 5355u ) {
        if( auto const rc = _dns.dissect( payload, v._end ); rc == nygma::dns::dns_dissect_rc::OK ) {
          _dns_count++;
          if( _dns._rr_count > 0 ) {
            for( std::size_t i = 0; i < _dns._rr_count; i++ ) {
              _labels_length[i] = _dns._rr[i]._label_len;
            }
            _engine_dns.scanv( _labels, _labels_length, _dns._rr_count, _matched_ids );
          }
        } else {
          // if dns dissecting failed, fallback to the other engine
          _engine.scan( payload, payload_size, _matched_ids );
        }
      } else {
        _engine.scan( payload, payload_size, _matched_ids );
      }
      for( auto const id : _matched_ids ) { _index->add( id, static_cast<std::uint32_t>( _offset ) ); }
      _udp_count++;

    } else if constexpr( std::is_same_v<T, dissect::tcp> ) {
      unsigned const len = ( unsigned( v._begin[12] ) >> 2 ) & ~0b11u;
      std::byte const* const payload = v._begin + len;
      auto payload_size = static_cast<std::size_t>( v._end - payload );
      auto sp = unsafe::rd16<BE>( v._begin );
      auto dp = unsafe::rd16<BE>( v._begin + 2 );
      if( sp == 53u || sp == 5353u || sp == 5355 || dp == 53u || dp == 5353u || dp == 5355u ) {
        if( auto const rc = _dns.dissect( payload + 2 /* dns-over-tcp length prefix */, v._end );
            rc == nygma::dns::dns_dissect_rc::OK ) {
          _dns_count++;
          if( _dns._rr_count > 0 ) {
            for( std::size_t i = 0; i < _dns._rr_count; i++ ) {
              _labels_length[i] = _dns._rr[i]._rdata_len;
            }
            _engine_dns.scanv( _labels, _labels_length, _dns._rr_count, _matched_ids );
          }
        } else {
          // if dns dissecting failed fall back to the other engine
          _engine.scan( payload, payload_size, _matched_ids );
        }
      } else {
        _engine.scan( payload, payload_size, _matched_ids );
      }
      for( auto const id : _matched_ids ) { _index->add( id, static_cast<std::uint32_t>( _offset ) ); }
      _tcp_count++;
    }
  }

  template <typename Cycler>
  inline void prepare( std::uint64_t const offset, Cycler const c ) noexcept {
    _matched_ids.clear();
    if( offset - _segment_offset > SEGMENTSZ ) {
      c( std::move( _index ), _segment_offset );
      _segment_offset = offset;
      _index = std::make_unique<index_type>();
    }
    // store relative offset to the beginning of a segment @ `_segment_offset`
    _offset = offset - _segment_offset;
    dissect::dissect_trace::rewind();
  }

  template <typename Cycler>
  inline void finish( Cycler const c ) noexcept {
    // provide the last stored `_segment_offset` to the cycler
    c( std::move( _index ), _segment_offset );
  }
};

} // namespace t3tch
