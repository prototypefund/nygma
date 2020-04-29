// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libriot/querying/query-lexer.hxx>

#include <memory>
#include <sstream>
#include <stdexcept>

namespace riot {

enum class kind {
  BINARY,
  QUERY,
  ID,
  IPV4,
  IPV6,
  NUM,
};

enum class binop {
  AND,
  OR,
  PLUS,
  MINUS,
  SLASH,
  BACKSLASH,
};

struct source_span {
  unsigned _offset;
  unsigned _size;

  static constexpr source_span const null() noexcept { return { 0, 0 }; }

  static constexpr source_span const from( token const tok ) noexcept {
    return { tok.offset(), tok.size() };
  }
};

namespace {

inline std::string_view const to_string( kind const t ) noexcept {
  switch( t ) {
    case kind::BINARY: return "BINARY";
    case kind::QUERY: return "QUERY";
    case kind::ID: return "ID";
    case kind::IPV6: return "IPV6";
    case kind::IPV4: return "IPV4";
    case kind::NUM: return "NUM";
  }
}

} // namespace

enum class query_method { LOOKUP, REVERSE_LOOKUP, COMBINED };

struct expression_coercion_error : std::runtime_error {
  expression_coercion_error( std::string const& msg ) : std::runtime_error( msg ) {}
  virtual ~expression_coercion_error() override = default;
};

struct node {
  virtual ~node() = default;
  virtual kind type() const noexcept = 0;

  void expect_kind( kind const t ) const {
    if( type() == t ) { return; }
    std::ostringstream msg;
    msg << "expression coercion failed: exptected = " << to_string( t )
        << " actual = " << to_string( type() );
    throw expression_coercion_error( msg.str() );
  }

  template <kind T, typename Visitor>
  void accept( Visitor const v );
};

using node_ptr = std::unique_ptr<node>;

template <kind T>
struct typed_node : public node {
  static constexpr kind TYPE = T;

  source_span _span;

  constexpr typed_node( source_span const span ) noexcept : _span{ span } {}

  kind type() const noexcept override { return type_impl(); }

  constexpr kind type_impl() const noexcept { return TYPE; }
};

struct ident : public typed_node<kind::ID> {
  token _tok;
  std::string _name;
  ident( token const tok, std::string_view const name )
    : typed_node<kind::ID>{ source_span::from( tok ) }, _tok{ tok }, _name{ name } {}
};

template <kind T, typename I>
struct literal : public typed_node<T> {
  I const _value;
  constexpr literal( source_span const span, I const value )
    : typed_node<T>{ span }, _value{ value } {}
};

struct ipv4 : public literal<kind::IPV4, std::uint32_t> {
  template <typename... Args>
  constexpr ipv4( Args&&... args )
    : literal<kind::IPV4, std::uint32_t>{ std::forward<Args>( args )... } {}
};

struct ipv6 : public literal<kind::IPV6, __uint128_t> {
  template <typename... Args>
  constexpr ipv6( Args&&... args )
    : literal<kind::IPV6, __uint128_t>{ std::forward<Args>( args )... } {}
};

struct number : public literal<kind::NUM, std::uint64_t> {
  template <typename... Args>
  constexpr number( Args&&... args )
    : literal<kind::NUM, std::uint64_t>{ std::forward<Args>( args )... } {}
};

struct query : public typed_node<kind::QUERY> {
  query_method _method;
  ident _index;
  node_ptr _what;
  std::uint64_t _limit;
};

struct binary : public typed_node<kind::BINARY> {
  binop _op;
  node_ptr _left;
  node_ptr _right;
};

template <kind T, typename Visitor>
void node::accept( Visitor const v ) {
  if constexpr( T == kind::ID ) {
    expect_kind( kind::ID );
    v( static_cast<ident&>( *this ) );
  } else if constexpr( T == kind::NUM ) {
    expect_kind( kind::NUM );
    v( static_cast<number&>( *this ) );
  }
}

namespace ast {

namespace {

inline node_ptr number( source_span const span, std::uint64_t const value ) noexcept {
  return std::make_unique<riot::number>( span, value );
}

inline node_ptr ipv4( source_span const span, std::uint32_t const value ) noexcept {
  return std::make_unique<riot::ipv4>( span, value );
}

} // namespace

} // namespace ast

} // namespace riot
