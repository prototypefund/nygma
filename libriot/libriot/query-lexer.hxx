// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <cctype>
#include <string_view>

namespace riot {

struct token_type {
  using type = unsigned;
  enum : type {
    LB,
    RB,
    LP,
    RP,
    LT,
    GT,
    LS,
    RS,
    EQ,
    ID,
    MINUS,
    STAR,
    PLUS,
    BACKSLASH,
    SLASH,
    AMP,
    OR,
    NUM,
    IPV4,
    IPV6,
    EOS,
    BAD
  };
};

struct token {
  static constexpr unsigned OMASK = 0xffffffff;
  static constexpr unsigned OSHIFT = 0;
  static constexpr unsigned SMASK = 0xffffff;
  static constexpr unsigned SSHIFT = 32;
  static constexpr unsigned TMASK = 0xff;
  static constexpr unsigned TSHIFT = SSHIFT + 24;

  // squeeze everything into a 64bit value
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

  template <token_type::type T, typename Predicate>
  constexpr token consume( Predicate const p ) noexcept {
    auto const begin = _offset;
    while( p( peek() ) ) { pop(); }
    return { T, begin, _offset - begin };
  }

  //--non-validating
  constexpr token consume_ipv4_literal() noexcept {
    auto constexpr p = []( auto const c ) noexcept { return c == '.' and ::isdigit( c ); };
    return consume<token_type::IPV4>( p );
  }

  //--non-validating
  constexpr token consume_ipv6_literal() noexcept {
    auto constexpr p = []( auto const c ) noexcept { return c == '.' or c == ':' or ::isxdigit( c ); };
    return consume<token_type::IPV6>( p );
  }

  //--non-validating
  constexpr token consume_literal() noexcept {
    auto const begin = _offset;
    auto expected_output_token = token_type::NUM;
    auto const p = [&]( auto const c ) noexcept {
      if( ::isdigit( c ) ) {
        return true;
      } else if( ::isxdigit( c ) ) {
        expected_output_token = token_type::IPV6;
        return true;
      } else if( c == '.' ) {
        expected_output_token = // if already in v6 do not switch back to v4
            expected_output_token == token_type::IPV6 ? token_type::IPV6 : token_type::IPV4;
        return true;
      } else if( c == ':' ) {
        expected_output_token = token_type::IPV6;
        return true;
      }
      return false;
    };
    while( p( peek() ) ) { pop(); }
    return { expected_output_token, begin, _offset - begin };
  }

  template <token_type::type T>
  constexpr token const one() noexcept {
    auto const begin = _offset;
    pop();
    return { T, begin, 1u };
  }

  template <typename Predicate>
  constexpr token const next( Predicate const p ) noexcept {
    while( p( peek() ) ) { pop(); }
    auto const begin = _offset;
    auto c = peek();
    switch( c ) {
      case '(': return one<token_type::LP>();
      case ')': return one<token_type::RP>();
      case '{': return one<token_type::LB>();
      case '}': return one<token_type::RB>();
      case '[': return one<token_type::LS>();
      case ']': return one<token_type::RS>();
      case '<': return one<token_type::LT>();
      case '>': return one<token_type::GT>();
      case '*': return one<token_type::STAR>();
      case '-': return one<token_type::MINUS>();
      case '/': return one<token_type::SLASH>();
      case '+': return one<token_type::PLUS>();
      case '=': return one<token_type::EQ>();
      case '&': return one<token_type::AMP>();
      case '\\': return one<token_type::BACKSLASH>();
      case ':': return consume_ipv6_literal();
      case 'a' ... 'f': return consume_ipv6_literal();
      case 'A' ... 'F': return consume_ipv6_literal();
      case '0' ... '9': return consume_literal();
      case -1: return { token_type::EOS, begin, _offset - begin };
      default:
        if( ::isalpha( c ) ) {
          return consume<token_type::ID>( []( auto const _ ) { return ::isalnum( _ ); } );
        }
        return { token_type::BAD, _offset, std::max( _offset - begin, 1ul ) };
    }
  }

  constexpr token const next() noexcept {
    return next( []( auto const c ) noexcept { return std::isspace( c ); } );
  }

  template <typename Predicate>
  constexpr void skip( Predicate const p ) noexcept {
    while( p( peek() ) ) { pop(); }
  }

  constexpr void skip_ws() noexcept {
    return skip( []( auto const c ) noexcept { return std::isspace( c ); } );
  }

  constexpr std::string_view slice_of( token const tok ) const noexcept {
    if( static_cast<std::size_t>( tok.offset() ) + tok.size() > _data.size() ) { return {}; }
    return { _data.data() + tok.offset(), tok.size() };
  }
};

} // namespace riot
