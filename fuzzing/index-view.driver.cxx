// SPDX-License-Identifier: BlueOak-1.0.0

#include <libriot/index-view.hxx>
#include <libunclassified/bytestring.hxx>

#include <vector>

extern "C" {
#include <arpa/inet.h>
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-prototypes"

// @see https://github.com/facebook/folly/blob/master/folly/Benchmark.h

template <typename T>
struct DoNotOptimizeAwayNeedsIndirect {
  using Decayed = typename std::decay<T>::type;

  // First two constraints ensure it can be an "r" operand.
  // std::is_pointer check is because callers seem to expect that
  // doNotOptimizeAway(&x) is equivalent to doNotOptimizeAway(x).
  constexpr static bool value = not std::is_trivially_copyable<Decayed>::value ||
      sizeof( Decayed ) > sizeof( long ) || std::is_pointer<Decayed>::value;
};

template <typename T>
auto doNotOptimizeAway( const T& datum ) ->
    typename std::enable_if<not DoNotOptimizeAwayNeedsIndirect<T>::value>::type {
  // The "r" constraint forces the compiler to make datum available
  // in a register to the asm block, which means that it must have
  // computed/loaded it.  We use this path for things that are <=
  // sizeof(long) (they have to fit), trivial (otherwise the compiler
  // doesn't want to put them in a register), and not a pointer (because
  // doNotOptimizeAway(&foo) would otherwise be a foot gun that didn't
  // necessarily compute foo).
  //
  // An earlier version of this method had a more permissive input operand
  // constraint, but that caused unnecessary variation between clang and
  // gcc benchmarks.
  asm volatile( "" ::"r"( datum ) );
}

template <typename T>
auto doNotOptimizeAway( const T& datum ) ->
    typename std::enable_if<DoNotOptimizeAwayNeedsIndirect<T>::value>::type {
  // This version of doNotOptimizeAway tells the compiler that the asm
  // block will read datum from memory, and that in addition it might read
  // or write from any memory location.  If the memory clobber could be
  // separated into input and output that would be preferrable.
  asm volatile( "" ::"m"( datum ) : "memory" );
}

namespace {

using bytestring_view = unclassified::bytestring_view;

} // namespace

extern "C" int LLVMFuzzerTestOneInput( uint8_t const* p, size_t const sz ) {
  try {
    auto const bs = bytestring_view{ reinterpret_cast<std::byte const*>( p ), sz };
    auto const index = riot::make_poly_index_view( bs );
    auto const v4 = "149.171.126.16";
    auto const ip = __builtin__bswap32( ::inet_addr( v4 ) );
    auto const a = index->lookup_forward_32( ip );
    auto const b = index->lookup_forward_32( 80 );
    auto const c = index->lookup_forward_64( static_cast<std::uint64_t>( ip ) );
    auto const d = index->lookup_forward_128( __uint128_t( ip ) );
    doNotOptimizeAway( a );
    doNotOptimizeAway( b );
    doNotOptimizeAway( c );
    doNotOptimizeAway( d );
    return 0;
  } catch( ... ) { return 0; }
}

#pragma clang diagnostic pop
