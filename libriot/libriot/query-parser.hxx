// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libriot/query-ast.hxx>
#include <libriot/query-lexer.hxx>

#include <deque>
#include <stdexcept>
#include <string_view>

namespace riot {

namespace detail {

struct precedence {
  using type = int;
  enum : type {
    MIN = -1,
    DEFAULT = 0,
    ASSIGNMENT = 100,
    COMPLEMENT = 200,
    INCLUSIVE = 300,
    EXCLUSIVE = 400,
    ADDITIVE = 500,
    MULTIPLCATIVE = 600,
    PREFIX = 700,
    POSTFIX = 800,
    CALL = 900,
    MAX = std::numeric_limits<type>::max(),
  };
};

struct parser {
  scanner _s;
  std::deque<token> _stack;
  explicit parser( std::string_view const query ) : _s{ query } {}

  parser( parser const& ) = delete;
  parser& operator=( parser const& ) = delete;

  token peek() noexcept {
    if( _stack.empty() ) { _stack.emplace_back( _s.next() ); }
    return _stack.front();
  }

  token pop() noexcept {
    if( _stack.empty() ) { _stack.emplace_back( _s.next() ); }
    auto const t = _stack.front();
    _stack.pop_front();
    return t;
  }

  void expect( token_type::type const expected ) {
    if( pop().type() not_eq expected ) {
      throw std::runtime_error( "parser-expect: unexpected token" );
    }
  }

  std::string_view slice_of( token const t ) const noexcept { return _s.slice_of( t ); }

  expression expression( precedence::type const precedence );
};

namespace parselet {

//--prefix-like-parselets-----------------------------------------------------

class prefix {
 public:
  virtual expression accept( parser& p, token const t ) const = 0;
  virtual detail::precedence::type precedence() const noexcept = 0;
  virtual ~prefix() = default;
};

class ident final : public prefix {
 private:
  ident() {}

 public:
  expression accept( parser& p, token const t ) const override {
    return ast::ident( source_span::from( t ), p.slice_of( t ) );
  }

  precedence::type precedence() const noexcept override { return precedence::DEFAULT; }

  static auto const& instance() noexcept {
    static ident _self;
    return _self;
  }
};

class parenthesized final : public prefix {
 private:
  parenthesized() {}

 public:
  expression accept( parser& p, [[maybe_unused]] token const t ) const override {
    auto e = p.expression( precedence::DEFAULT );
    p.expect( token_type::RP );
    return e;
  }

  precedence::type precedence() const noexcept override { return precedence::DEFAULT; }

  static auto const& instance() noexcept {
    static parenthesized _self;
    return _self;
  }
};

class number final : public prefix {
 private:
  number() {}

 public:
  expression accept( parser& p, token const t ) const override {
    std::string n{ p.slice_of( t ) };
    return ast::number( source_span::from( t ), std::stoull( n ) );
  }

  precedence::type precedence() const noexcept override { return precedence::DEFAULT; }

  static auto const& instance() noexcept {
    static number _self;
    return _self;
  }
};

//--infix-like-parselets------------------------------------------------------

class infix {
 public:
  virtual expression accept( parser& p, expression e, token const t ) const = 0;
  virtual precedence::type precedence() const noexcept = 0;
  virtual ~infix() = default;
};

template<query_method Qm, token_type::type ClosingParen>
struct query final : public infix {
  using qm = query_method;

 private:
  query() {}

 public:
  expression accept( parser& p, expression index, token const t ) const override {
    auto const span = source_span::from( t );
    auto what = p.expression( precedence::DEFAULT );
    p.expect( ClosingParen );
    return ast::lookup( span, std::move( index ), Qm, std::move( what ) );
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static query _self;
    return _self;
  }
};

template <binop Op, precedence::type Precedence>
struct binary final : public infix {
 private:
  binary() {}

 public:
  expression accept( parser& p, expression a, token const t ) const override {
    auto const span = source_span::from( t );
    auto b = p.expression( precedence::DEFAULT );
    return ast::binary( span, std::move( a ), Op, std::move( b ) );
  }

  precedence::type precedence() const noexcept override { return Precedence; }

  static auto const& instance() noexcept {
    static binary _self;
    return _self;
  }
};

class eos final : public infix {
 private:
  eos() {}

 public:
  expression accept( parser&, expression, token const ) const override {
    throw std::runtime_error( "eos-parselet: unexpected end of input " );
  }

  precedence::type precedence() const noexcept override { return precedence::MIN; }

  static auto const& instance() noexcept {
    static eos _self;
    return _self;
  }
};

} // namespace parselet

namespace {

//--precedence-resolution-----------------------------------------------------

parselet::prefix const& parselet_for_prefixes( token const t ) {
  switch( t.type() ) {
    case token_type::ID: return parselet::ident::instance();
    case token_type::NUM: return parselet::number::instance();
    case token_type::LP: return parselet::parenthesized::instance();
    default: throw std::runtime_error( "parslet-prefix-select: unexpected token" );
  }
}

parselet::infix const& parselet_for_infixes( token const t ) {
  using b = binop;
  using p = precedence;
  using q = query_method;
  switch( t.type() ) {
    case token_type::LP: return parselet::query<q::FORWARD, token_type::RP>::instance();
    case token_type::RP: return parselet::eos::instance();
    case token_type::EOS: return parselet::eos::instance();
    case token_type::BACKSLASH: return parselet::binary<b::COMPLEMENT, p::COMPLEMENT>::instance();
    case token_type::MINUS: return parselet::binary<b::COMPLEMENT, p::ADDITIVE>::instance();
    case token_type::PLUS: return parselet::binary<b::UNION, p::INCLUSIVE>::instance();
    case token_type::AMP: return parselet::binary<b::INTERSECTION, p::EXCLUSIVE>::instance();
    default: throw std::runtime_error( "parselet-infix-select: unexpected token" );
  }
}

} // namespace

//--pratt-parser-loop
expression parser::expression( precedence::type const precedence ) {
  auto const& parselet = parselet_for_prefixes( peek() );
  auto e = parselet.accept( *this, pop() );
  while( true ) {
    auto const& infixlet = parselet_for_infixes( peek() );
    if( precedence >= infixlet.precedence() ) { return e; }
    e = infixlet.accept( *this, std::move( e ), pop() );
  }
}

} // namespace detail

namespace {

expression const parse( std::string_view const query ) {
  detail::parser parser{ query };
  return parser.expression( detail::precedence::DEFAULT );
}
} // namespace
} // namespace riot
