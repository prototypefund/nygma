// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libriot/querying/query-lexer.hxx>

#include <memory>

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
};

enum class query_method { LOOKUP, REVERSE_LOOKUP, COMBINED };

struct node {
  virtual ~node() = default;
  virtual kind type() const noexcept = 0;
};

using node_ptr = std::unique_ptr<node>;

struct source_span {
  unsigned _offset;
  unsigned _size;
};

template <kind T>
struct typed_node : public node {
  static constexpr kind TYPE = T;

  source_span _span;

  constexpr typed_node( source_span const span ) noexcept : _span{ span } {}

  kind type() const noexcept override { return type_impl(); }

  constexpr kind type_impl() const noexcept { return TYPE; }
};

struct ident : public typed_node<kind::ID> {
  std::string _name;
  token _tok;
  constexpr ident( token const tok, std::string_view const name ) : _tok{ tok }, _name{ name } {}
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

namespace ast {

constexpr riot::number number( source_span const span, std::uint64_t const value ) noexcept {
  return riot::number{ span, value };
}

} // namespace ast

} // namespace riot
