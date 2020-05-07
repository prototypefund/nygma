// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <algorithm>
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

  static container_type set_complement( container_type const& a, container_type const& b ) noexcept {
    container_type r;
    std::set_difference( a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::back_inserter( r ) );
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

//--result-sets---------------------------------------------------------------

template <typename Traits, detail::resultset_kind K>
struct resultset {
  using traits_type = Traits;
  using value_type = typename traits_type::value_type;
  using container_type = typename traits_type::container_type;
  using resultset_type = resultset<traits_type, K>;
  using segment_offset_type = std::uint64_t;
  static constexpr detail::resultset_kind KIND = K;

  segment_offset_type const _segment_offset;
  bool _success;
  container_type _values;

 public:
  template <typename... Args>
  constexpr explicit resultset( segment_offset_type const so, Args&&... args )
    : _segment_offset{ so }, _success{ false }, _values{ std::forward<Args>( args )... } {}

  template <typename... Args>
  constexpr explicit resultset( segment_offset_type const so, bool const rc, Args&&... args )
    : _segment_offset{ so }, _success{ rc }, _values{ std::forward<Args>( args )... } {}

  resultset( resultset& ) = delete;
  resultset& operator=( resultset& ) = delete;

  resultset( resultset const&& ) noexcept = default;
  resultset& operator=( resultset const&& ) noexcept = default;

  resultset_type set_union( resultset_type const& o ) const noexcept {
    return resultset_type{ _segment_offset, traits_type::set_union( _values, o._values ) };
  }

  resultset_type set_intersection( resultset_type const& o ) const noexcept {
    return resultset_type{ _segment_offset, traits_type::set_intersection( _values, o._values ) };
  }

  resultset_type set_complement( resultset_type const& o ) const noexcept {
    return resultset_type{ _segment_offset, traits_type::set_complement( _values, o._values ) };
  }

  template <typename OT, detail::resultset_kind OK>
  resultset_type operator-( resultset<OT, OK> const& o ) const noexcept {
    if constexpr( std::is_same_v<OT, traits_type> and OK == KIND ) {
      if( _segment_offset == o._segment_offset ) { return set_complement( o ); }
    }
    return resultset_type{ _segment_offset };
  }

  template <typename OT, detail::resultset_kind OK>
  resultset_type operator+( resultset<OT, OK> const& o ) const noexcept {
    if constexpr( std::is_same_v<OT, traits_type> and OK == KIND ) {
      if( _segment_offset == o._segment_offset ) { return set_union( o ); }
    }
    return resultset_type{ _segment_offset };
  }

  template <typename OT, detail::resultset_kind OK>
  resultset_type operator&( resultset<OT, OK> const& o ) const noexcept {
    if constexpr( std::is_same_v<OT, traits_type> and OK == KIND ) {
      if( _segment_offset == o._segment_offset ) { return set_intersection( o ); }
    }
    return resultset_type{ _segment_offset };
  }

  template <typename OT, detail::resultset_kind OK>
  resultset<OT, OK> const& coerce( resultset<OT, OK> const& dummy ) {
    if constexpr( std::is_same_v<OT, traits_type> and OK == KIND ) {
      return { *this, _success };
    }
    return dummy;
  }

  explicit constexpr operator bool() const noexcept { return _success; }
  constexpr void success( bool const rc ) { _success = rc; }
  constexpr auto kind() const noexcept { return KIND; }
  constexpr auto size() const noexcept { return _values.size(); }
  constexpr auto empty() const noexcept { return _values.empty(); }
  constexpr auto const& values() const noexcept { return _values; }
  constexpr auto segment_offset() const noexcept { return _segment_offset; }

  constexpr auto cbegin() const noexcept { return _values.cbegin(); }
  constexpr auto cend() const noexcept { return _values.cend(); }

  static auto const& dummy() noexcept {
    static const resultset_type DUMMY = resultset_type{ 0 };
    return DUMMY;
  }
};

struct poly_resultset {

  struct interface {
    virtual ~interface() = default;
    virtual std::size_t size() const noexcept = 0;
    //virtual
  };

  std::unique_ptr<interface> _resultset;
};

} // namespace riot
