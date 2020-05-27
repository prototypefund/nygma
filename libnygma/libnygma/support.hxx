// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <new>

namespace {

template <std::uint16_t X>
constexpr std::uint16_t be16() {
  if constexpr( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ ) {
    return ( X );
  } else if constexpr( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ) {
    return ( __builtin_bswap16( X ) );
  }
}

template <typename... Args>
constexpr void unused( Args&&... ) {}

template <std::size_t N>
struct is_power_of_two {
  static constexpr bool value = ( N > 1 ) && ! ( N & ( N - 1 ) );
};

template <std::size_t N>
constexpr bool is_power_of_two_v = is_power_of_two<N>::value;

constexpr double to_MiB( std::size_t const sz ) { return ( static_cast<double>( sz ) / ( 1 << 20 ) ); }

constexpr double to_Mops( std::size_t const sz, double const delta_t ) {
  return ( static_cast<double>( sz ) / ( ( 1 << 20 ) * delta_t ) );
}

constexpr double to_Mbps( std::size_t const sz, double const delta_t ) {
  return ( static_cast<double>( sz * 8 ) / ( ( 1 << 20 ) * delta_t ) );
}

constexpr double to_KiB( std::size_t const sz ) { return ( static_cast<double>( sz ) / ( 1 << 10 ) ); }

template <typename T>
constexpr T align_up( T v, T align ) {
  return ( v + align - 1 ) & ~( align - 1 );
}

template <typename T, std::size_t Align>
constexpr T* align_up( T* v ) {
  static_assert( sizeof( T ) == 1, "align byte pointers only" );
  static_assert( is_power_of_two_v<Align>, "Align must be power of two" );
  return reinterpret_cast<T*>(
      align_up( reinterpret_cast<uintptr_t>( v ), reinterpret_cast<uintptr_t>( Align ) ) );
}

// returns microseconds since epoch
inline std::int64_t timestamp_ms() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch() )
      .count();
}

} // namespace

namespace std {

constexpr std::size_t CACHE_ALIGN_FALLBACK = 64;

// libc++ 8.0 and later define __cpp_lib_hardware_interference_size but don't
// actually implement it
#if( __cplusplus >= 201703L ) && defined( __cpp_lib_hardware_interference_size ) &&                   \
    defined( _LIBCPP_VERSION )
constexpr std::size_t hardware_destructive_interference_size = CACHE_ALIGN_FALLBACK;
#endif

#if ! defined( __cpp_lib_hardware_interference_size )
constexpr std::size_t hardware_destructive_interference_size = CACHE_ALIGN_FALLBACK;
#endif

} // namespace std

namespace nygma {

constexpr std::size_t CACHE_ALIGN = std::hardware_destructive_interference_size;

}
