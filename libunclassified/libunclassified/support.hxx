// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <cstdint>
#include <cstdlib>

namespace unclassified {

template <std::size_t N>
struct is_power_of_two {
  static constexpr bool value = ( N > 1 ) && ! ( N & ( N - 1 ) );
};
template <std::size_t N>
constexpr bool is_power_of_two_v = is_power_of_two<N>::value;

template <typename T>
constexpr T align_up( T v, T align ) {
  return ( v + align - 1 ) & ~( align - 1 );
}

template <typename T, std::size_t Align>
constexpr T* align_up( T* v ) {
  static_assert( sizeof( T ) == 1, "align byte pointers only" );
  static_assert( is_power_of_two_v<Align>, "Align must be power of two" );
  return reinterpret_cast<T*>(
      align_up( reinterpret_cast<std::uintptr_t>( v ), reinterpret_cast<std::uintptr_t>( Align ) ) );
}

template <typename... Args>
constexpr void unused( Args&&... ) {}

namespace {
constexpr std::size_t CACHE_ALIGN = 64u;
}

} // namespace unclassified
