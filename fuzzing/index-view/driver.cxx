// SPDX-License-Identifier: UNLICENSE

#include <libnygma/dns.hxx>
#include <libnygma/dns-trace.hxx>
#include <libnygma/toeplitz.hxx>
#include <libnygma/pcap-view.hxx>
#include <libriot/indexing/index-view.hxx>
#include <libunclassified/bytestring.hxx>

#include <algorithm>
#include <vector>

extern "C" {
#include <arpa/inet.h>
}

namespace unsafe = unclassified::unsafe;
namespace dissect = nygma::dissect;
namespace toeplitz = nygma::toeplitz;
namespace dns = nygma::dns;

using hash_type = toeplitz::toeplitz_scalar_lut<toeplitz::rss_key_symmetric>;
using dissect_tag = dissect::dissect_tag;
using dns_trace = dns::dns_trace;
using block_view = nygma::block_view;
using bytestring_view = unclassified::bytestring_view;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"

extern "C" int LLVMFuzzerTestOneInput( uint8_t* const p, size_t const sz ) {
  auto const data = bytestring_view{ reinterpret_cast<std::byte const*>( p ), sz };
  std::size_t _total_packets{ 0 };
  std::size_t _total_bytes{ 0 };
  std::uint64_t _first_seen{ std::numeric_limits<std::uint64_t>::max() };
  std::uint64_t _last_seen{ 0 };
  std::uint64_t _dns_count{ 0 };

  dns_trace _trace{};
  hash_type _hash_policy{};
  dns::dns_t _dns;

  nygma::pcap::with( std::move( data ), [&]( auto const& pcap ) noexcept {
    if( not pcap.valid() ) { return; }
    pcap.for_each( [&]( auto const& pkt, auto const ) {
      _trace.rewind();
      dissect::dissect_en10mb( _hash_policy, _trace, pkt._slice );
      if( _trace._assume_dns && _trace.valid() ) {
        auto const rc = _dns.dissect( _trace._dns_begin, _trace._end );
        if( rc == dns::dns_dissect_rc::OK ) { _dns_count++; }
      }
      _total_packets++;
      _total_bytes += pkt._slice.size();
      _first_seen = std::min( pkt._stamp, _first_seen );
      _last_seen = std::max( pkt._stamp, _last_seen );
    } );
  } );

  return 0;
}

#pragma clang diagnostic pop
