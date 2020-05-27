// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <algorithm>
#include <cstddef>

namespace riot::integer {

template <std::size_t N>
struct is_power_of_two {
  static constexpr bool value = ( N > 1 ) && ! ( N & ( N - 1 ) );
};

template <std::size_t N>
constexpr bool is_power_of_two_v = is_power_of_two<N>::value;

template <std::size_t N>
inline std::size_t align_up( std::size_t const x ) noexcept {
  static_assert( is_power_of_two_v<N> );
  return ( x + ( N - 1 ) ) & ~( N - 1 );
}

template <typename I, std::size_t N>
inline void fill_up( I* p, std::size_t const n ) noexcept {
  auto aligned = align_up<N>( n );
  std::fill_n( p + n, aligned - n, p[n - 1] );
}

} // namespace riot::integer
