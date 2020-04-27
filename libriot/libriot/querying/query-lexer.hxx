// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <cctype>
#include <string_view>

namespace riot {

struct token_type {
  using type = unsigned;
  enum : type { LB, RB, LP, RP, LT, GT, LS, RS, NAME, AND, OR, EOS, BAD };
};

struct token {
  static constexpr unsigned OMASK = 0xffffffff;
  static constexpr unsigned OSHIFT = 0;
  static constexpr unsigned SMASK = 0xffffff;
  static constexpr unsigned SSHIFT = 32;
  static constexpr unsigned TMASK = 0xff;
  static constexpr unsigned TSHIFT = SSHIFT + 24;

  std::uint64_t const _token;

  constexpr token( token_type::type token, std::size_t offset, std::size_t size ) noexcept
    : _token{
          ( static_cast<std::uint64_t>( token & TMASK ) << TSHIFT ) | ( ( size & SMASK ) << SSHIFT ) |
          ( ( offset & OMASK ) << OSHIFT ) } {}

  constexpr unsigned offset() const noexcept { return ( _token >> OSHIFT ) & OMASK; }

  constexpr unsigned type() const noexcept { return ( _token >> TSHIFT ) & TMASK; }

  constexpr unsigned size() const noexcept { return ( _token >> SSHIFT ) & SMASK; }

  std::string_view slice( std::string_view input ) const noexcept {
    if( not ( offset() + size() < input.size() ) ) { return {}; }
    return { input.data() + offset(), size() };
  }
};

struct scanner {
  std::string_view _data;
  std::size_t _offset;

  scanner( std::string_view data ) : _data{ data }, _offset{ 0 } {}

  constexpr int peek() noexcept {
    if( _offset >= _data.size() ) { return -1; }
    return static_cast<int>(
        static_cast<std::make_unsigned_t<std::string_view::value_type const>>( _data[_offset] ) );
  }

  constexpr void pop() noexcept { _offset++; }

  template <typename IsWhiteSpace>
  constexpr token const next( IsWhiteSpace const predicate ) noexcept {
    while( predicate( peek() ) ) {
      pop();
    }
    auto const _begin = _offset;
    switch( peek() ) {
      case '(': pop(); return { token_type::LP, _begin, 1u };
      case ')': pop(); return { token_type::RP, _begin, 1u };
      case -1: return { token_type::EOS, _begin, _offset - _begin };
      default: return { token_type::BAD, _offset, std::max( _offset - _begin, 1ul ) };
    }
  }

  constexpr token const next() noexcept {
    return next( []( auto const c ) noexcept { return std::isspace( c ); } );
  }
};

} // namespace riot
