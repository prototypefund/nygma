// SPDX-License-Identifier: BlueOak-1.0.0

#include <libnygma/dns-trace.hxx>
#include <libnygma/dns.hxx>
#include <libnygma/pcap-view.hxx>
#include <libnygma/toeplitz.hxx>
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
using block_view = nygma::block_view_2m;
using bytestring_view = unclassified::bytestring_view;

int main( int argc, char const** argv ) {
  if( argc != 2 ) { return EXIT_FAILURE; }
  char const* const file = argv[1];
  auto data = std::make_unique<nygma::block_view_2m>( file, nygma::block_flags::rd );
  std::size_t _total_packets{ 0 };
  std::size_t _total_bytes{ 0 };
  std::uint64_t _first_seen{ std::numeric_limits<std::uint64_t>::max() };
  std::uint64_t _last_seen{ 0 };
  std::uint64_t _total_dns_packets{ 0 };

  dns_trace _trace{};
  hash_type _hash_policy{};
  dns::dns_t _dns;

  nygma::pcap::with( std::move( data ), [&]( auto const& pcap ) noexcept {
    if( not pcap.valid() ) {
      std::clog << "invalid pcap" << std::endl;
      return;
    }
    pcap.for_each( [&]( auto const& pkt, auto const ) {
      _trace.rewind();
      dissect::dissect_en10mb( _hash_policy, _trace, pkt._slice );
      if( _trace._assume_dns && _trace.valid() ) {
        auto const rc = _dns.dissect( _trace._dns_begin, _trace._end );
        if( rc == dns::dns_dissect_rc::OK ) { _total_dns_packets++; }
      }
      _total_packets++;
      _total_bytes += pkt._slice.size();
      _first_seen = std::min( pkt._stamp, _first_seen );
      _last_seen = std::max( pkt._stamp, _last_seen );
    } );
  } );

  std::clog << "total_packets = " << _total_packets << std::endl;
  std::clog << "dns_count = " << _total_dns_packets << std::endl;

  return 0;
}
