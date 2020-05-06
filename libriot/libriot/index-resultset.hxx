// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <cstdint>
#include <vector>

namespace riot {

namespace detail {

enum class resultset_kind { FORWARD, REVERSE };

template <typename T>
struct std_vector_traits {
  using value_type = T;
  using container_type = std::vector<value_type>;

  static container_type set_union( container_type const& a, container_type const& b ) noexcept {
    container_type r;
    r.reserve( a.size() + b.size() );

    if( a.empty() ) {
      r = b;
      return r;
    } else if( b.empty() ) {
      r = a;
      return r;
    }

    auto it_a = a.cbegin();
    auto it_b = b.cbegin();

    // {a} and {b} both not empty
    //
    r.push_back( std::min( *it_a, *it_b ) );
    while( it_a != a.cend() and it_b != b.cend() ) {
      if( *it_a < *it_b ) {
        if( r.back() < *it_a ) { r.push_back( *it_a ); }
        ++it_a;
      } else {
        if( r.back() < *it_b ) { r.push_back( *it_b ); }
        ++it_b;
      }
    }

    while( it_a != a.cend() ) {
      r.push_back( *it_a );
      ++it_a;
    }

    while( it_b != b.cend() ) {
      r.push_back( *it_b );
      ++it_b;
    }

    return r;
  }

  static container_type set_complement( container_type const&, container_type const& ) noexcept {
    container_type r;
    return r;
  }
  static container_type set_intersection( container_type const& a, container_type const& b ) noexcept {
    container_type r;

    if( a.empty() || b.empty() ) { return r; }

    auto it_a = a.cbegin();
    auto it_b = b.cbegin();

    while( true ) {
      while( *it_a < *it_b ) {
      skip_compare:
        if( ++it_a == a.cend() ) { return r; }
      }
      while( *it_a > *it_b ) {
        if( ++it_b == b.cend() ) { return r; }
      }
      if( *it_a == *it_b ) {
        r.push_back( *it_a );
        if( ++it_a == a.cend() or ++it_b == b.cend() ) { return r; }
      } else {
        goto skip_compare;
      }
    }

    // notreached
    return r;
  }
};

} // namespace detail

template <typename Traits, detail::resultset_kind K = detail::resultset_kind::FORWARD>
struct resultset {
  static constexpr detail::resultset_kind KIND = K;
  using traits_type = Traits;
  using container_type = typename traits_type::container_type;
  using resultset_type = resultset<traits_type>;
  using segment_offset_type = std::uint64_t;

  segment_offset_type const _segment_offset;
  container_type _values;

 public:
  template <typename... Args>
  explicit resultset( segment_offset_type const so, Args&&... args )
    : _segment_offset{ so }, _values{ std::forward<Args>( args )... } {}

  resultset( resultset& ) = delete;
  resultset& operator=( resultset& ) = delete;

  resultset( resultset const&& ) noexcept = default;
  resultset& operator=( resultset const&& ) noexcept = default;

  resultset<traits_type> operator+( resultset<traits_type> const& o ) const noexcept {
    return resultset_type{ _segment_offset, traits_type::set_union( _values, o._values ) };
  }

  resultset<traits_type> operator&( resultset<traits_type> const& o ) const noexcept {
    return resultset_type{ _segment_offset, traits_type::set_intersection( _values, o._values ) };
  }

  constexpr auto kind() const noexcept { return KIND; }
  constexpr auto size() const noexcept { return _values.size(); }
  constexpr auto empty() const noexcept { return _values.empty(); }
  constexpr auto const& values() const noexcept { return _values; }
  constexpr auto segment_offset() const noexcept { return _segment_offset; }
};

} // namespace riot
