// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <algorithm>
#include <cstdint>
#include <unordered_map>
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
    std::set_union( a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::back_inserter( r ) );
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

  segment_offset_type _segment_offset;
  bool _success;
  container_type _values;

 public:
  template <typename... Args>
  constexpr explicit resultset( segment_offset_type const so, Args&&... args )
    : _segment_offset{ so }, _success{ false }, _values{ std::forward<Args>( args )... } {}

  template <typename... Args>
  constexpr explicit resultset( segment_offset_type const so, bool const rc, Args&&... args )
    : _segment_offset{ so }, _success{ rc }, _values{ std::forward<Args>( args )... } {}

  constexpr resultset() : _segment_offset{ 0 }, _success{ false } {}

  resultset( resultset const& ) = delete;
  resultset& operator=( resultset const& ) = delete;

  resultset( resultset&& ) noexcept = default;
  resultset& operator=( resultset&& o ) noexcept = default;

  resultset_type set_union( resultset_type const& o ) const noexcept {
    return resultset_type{ _segment_offset, true, traits_type::set_union( _values, o._values ) };
  }

  resultset_type set_intersection( resultset_type const& o ) const noexcept {
    return resultset_type{
        _segment_offset,
        true,
        traits_type::set_intersection( _values, o._values ) };
  }

  resultset_type set_complement( resultset_type const& o ) const noexcept {
    return resultset_type{ _segment_offset, true, traits_type::set_complement( _values, o._values ) };
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
    if constexpr( std::is_same_v<OT, traits_type> and OK == KIND ) { return { *this, _success }; }
    return dummy;
  }

  explicit constexpr operator bool() const noexcept { return _success; }
  constexpr void success( bool const rc ) { _success = rc; }
  constexpr auto kind() const noexcept { return KIND; }
  constexpr auto size() const noexcept { return _values.size(); }
  constexpr auto empty() const noexcept { return _values.empty(); }
  constexpr auto const& values() const noexcept { return _values; }
  constexpr auto& values() noexcept { return _values; }
  constexpr auto segment_offset() const noexcept { return _segment_offset; }

  constexpr auto cbegin() const noexcept { return _values.cbegin(); }
  constexpr auto cend() const noexcept { return _values.cend(); }

  static constexpr resultset_type none() noexcept { return resultset{}; }

  template <typename T = resultset_type>
  static constexpr T combine_and( T const& a, T const& b ) noexcept {
    return a & b;
  }

  template <typename T = resultset_type>
  static constexpr T combine_or( T const& a, T const& b ) noexcept {
    return a + b;
  }

  template <typename T = resultset_type>
  static constexpr T combine_complement( T const& a, T const& b ) noexcept {
    return a - b;
  }

  static auto const& dummy() noexcept {
    static const resultset_type DUMMY = resultset_type{ 0 };
    return DUMMY;
  }
};

template <typename ResultSet>
struct sparse_resultset {
  using resultset_type = ResultSet;

  using value_type = typename resultset_type::value_type;
  using container_type = std::unordered_map<value_type, resultset_type>;
  using segment_offset_type = typename resultset_type::segment_offset_type;

  segment_offset_type _segment_offset;
  container_type _values;

  explicit sparse_resultset() : _segment_offset{ 0u } {}

  explicit sparse_resultset( segment_offset_type const segment_offset )
    : _segment_offset{ segment_offset } {}

  void bind( value_type const offset, resultset_type&& values ) noexcept {
    _values[offset] = _values[offset] + values;
  }

  template <auto CombineVertical, auto CombineHorizontal>
  static constexpr resultset_type combine(
      sparse_resultset<resultset_type> const& a, sparse_resultset<resultset_type> const& b ) noexcept {
    resultset_type result{ a._segment_offset, true };
    for( auto&& [offset, results_a] : a._values ) {
      auto it = b._values.find( offset );
      if( it == b._values.end() ) { continue; }
      result = CombineVertical( result, CombineHorizontal( results_a, it->second ) );
    }
    return result;
  }
};

} // namespace riot
