// SPDX-License-Identifier: UNLICENSE

#include <libriot/index-view.hxx>
#include <libunclassified/bytestring.hxx>

#include <vector>

extern "C" {
#include <arpa/inet.h>
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"

namespace {

using bytestring_view = unclassified::bytestring_view;

} // namespace

extern "C" int LLVMFuzzerTestOneInput( uint8_t const* p, size_t const sz ) {
  try {
    auto const bs = bytestring_view{ reinterpret_cast<std::byte const*>( p ), sz };
    auto const index = riot::make_poly_index_view( bs );
    auto const v4 = "149.171.126.16";
    auto const ip = __bswap32( ::inet_addr( v4 ) );
    auto const a = index->lookup_forward_32( ip );
    auto const b = index->lookup_forward_32( 80 );
    auto const c = index->lookup_forward_64( static_cast<std::uint64_t>( ip ) );
    auto const d = index->lookup_forward_128( __uint128_t( ip ) );
    return 0;
  } catch( ... ) { return 0; }
}

#pragma clang diagnostic pop
