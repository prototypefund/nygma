// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libriot/query-lexer.hxx>

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
  COMPLEMENT,
  INTERSECTION,
  UNION,
};

struct source_span {
  unsigned _offset;
  unsigned _size;

  static constexpr source_span const null() noexcept { return { 0, 0 }; }

  static constexpr source_span const from( token const tok ) noexcept {
    return { tok.offset(), tok.size() };
  }
};

// clang-format off
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded( Ts... ) -> overloaded<Ts...>;
// clang-format on

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

enum class query_method { FORWARD, REVERSE, COMBINED };

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
    msg << "expression coercion failed: expected = " << to_string( t )
        << " actual = " << to_string( type() );
    throw expression_coercion_error( msg.str() );
  }

  template <kind T, typename Visitor>
  void accept( Visitor const v );

  template <kind T, typename Visitor>
  auto eval( Visitor const v );

  template <typename Visitor>
  auto eval( Visitor&& v ) const;
};

using expression = std::unique_ptr<node>;

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
  ident( source_span const span, std::string_view const name )
    : typed_node<kind::ID>{ span }, _name{ name } {}
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
  expression _name;
  query_method _method;
  expression _what;
  std::uint64_t _limit{ 0 };
  template <typename N, typename T>
  query( source_span const span, N&& name, query_method const method, T&& what )
    : typed_node<kind::QUERY>{ span },
      _name{ std::forward<N>( name ) },
      _method{ method },
      _what{ std::forward<T>( what ) } {}
};

struct binary : public typed_node<kind::BINARY> {
  binop _op;
  expression _a;
  expression _b;
  template <typename A, typename B>
  binary( source_span const span, A&& a, binop const op, B&& b )
    : typed_node<kind::BINARY>{ span },
      _op{ op },
      _a{ std::forward<A>( a ) },
      _b{ std::forward<B>( b ) } {}
};

template <kind T, typename Visitor>
void node::accept( Visitor const v ) {
  if constexpr( T == kind::ID ) {
    expect_kind( kind::ID );
    v( static_cast<ident&>( *this ) );
  } else if constexpr( T == kind::NUM ) {
    expect_kind( kind::NUM );
    v( static_cast<number&>( *this ) );
  } else if constexpr( T == kind::BINARY ) {
    expect_kind( kind::BINARY );
    v( static_cast<binary&>( *this ) );
  } else if constexpr( T == kind::IPV4 ) {
    expect_kind( kind::IPV4 );
    v( static_cast<ipv4&>( *this ) );
  } else if constexpr( T == kind::IPV6 ) {
    expect_kind( kind::IPV6 );
    v( static_cast<ipv6&>( *this ) );
  } else if constexpr( T == kind::QUERY ) {
    expect_kind( kind::QUERY );
    v( static_cast<query&>( *this ) );
  }
}

template <kind T, typename Visitor>
auto node::eval( Visitor const v ) {
  if constexpr( T == kind::ID ) {
    expect_kind( kind::ID );
    return v( static_cast<ident&>( *this ) );
  } else if constexpr( T == kind::NUM ) {
    expect_kind( kind::NUM );
    return v( static_cast<number&>( *this ) );
  } else if constexpr( T == kind::BINARY ) {
    expect_kind( kind::BINARY );
    return v( static_cast<binary&>( *this ) );
  } else if constexpr( T == kind::IPV4 ) {
    expect_kind( kind::IPV4 );
    return v( static_cast<ipv4&>( *this ) );
  } else if constexpr( T == kind::IPV6 ) {
    expect_kind( kind::IPV6 );
    return v( static_cast<ipv6&>( *this ) );
  } else if constexpr( T == kind::QUERY ) {
    expect_kind( kind::QUERY );
    return v( static_cast<query&>( *this ) );
  }
}

template <typename Visitor>
auto node::eval( Visitor&& v ) const {
  switch( type() ) {
    case kind::ID: return v( static_cast<ident const&>( *this ) );
    case kind::NUM: return v( static_cast<number const&>( *this ) );
    case kind::IPV4: return v( static_cast<ipv4 const&>( *this ) );
    case kind::IPV6: return v( static_cast<ipv6 const&>( *this ) );
    case kind::BINARY: return v( static_cast<binary const&>( *this ) );
    case kind::QUERY: return v( static_cast<query const&>( *this ) );
  }
}

namespace ast {

namespace {

inline expression number( source_span const span, std::uint64_t const value ) noexcept {
  return std::make_unique<riot::number>( span, value );
}

inline expression ipv4( source_span const span, std::uint32_t const value ) noexcept {
  return std::make_unique<riot::ipv4>( span, value );
}

inline expression ipv6( source_span const span, __uint128_t const value ) noexcept {
  return std::make_unique<riot::ipv6>( span, value );
}

inline expression ident( source_span const span, std::string_view const name ) noexcept {
  return std::make_unique<riot::ident>( span, name );
}

template <typename A, typename B>
inline expression binary( source_span const span, A&& a, binop const op, B&& b ) noexcept {
  return std::make_unique<riot::binary>( span, std::forward<A>( a ), op, std::forward<B>( b ) );
}

template <typename N, typename T>
inline expression lookup( source_span const span, N&& name, query_method const m, T&& what ) noexcept {
  return std::make_unique<riot::query>( span, std::forward<N>( name ), m, std::forward<T>( what ) );
}

template <typename N, typename T>
inline expression lookup_forward( source_span const span, N&& name, T&& what ) noexcept {
  using qm = query_method;
  return lookup( span, std::forward<N>( name ), qm::FORWARD, std::forward<T>( what ) );
}

} // namespace

} // namespace ast

} // namespace riot
