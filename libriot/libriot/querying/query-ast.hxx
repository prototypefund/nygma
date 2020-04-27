// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libriot/querying/query-lexer.hxx>

#include <memory>

namespace riot {

enum class kind {
  BINARY,
  UNARY,
  ID,
  IPV4,
  IPV6,
  NUM,
};

enum class binop {
  AND,
  OR,
};

enum class query { FWD, REV, WIDEN };

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

  kind type() const noexcept override { return type_impl(); }

  constexpr kind type_impl() const noexcept { return TYPE; }
};

struct ident : public typed_node<kind::ID> {
  std::string _name;
  token _tok;
};

struct unary : public typed_node<kind::UNARY> {
  query _query;
  node_ptr _expr;
};

struct binary : public typed_node<kind::BINARY> {
  binop _op;
  node_ptr _left;
  node_ptr _right;
};

namespace ast {}

} // namespace riot
