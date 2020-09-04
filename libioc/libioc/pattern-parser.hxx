// License: RoundRobin-1.0.0

#pragma once

#include <libioc/pattern-lexer.hxx>

#include <deque>
#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace ioc {

struct pattern_database {

  using pattern_id = unsigned;
  using pattern_data = std::map<std::string, std::string>;
  using pattern_entry = std::pair<pattern_id, pattern_data>;
  using pattern = std::unique_ptr<pattern_entry>;

  std::vector<pattern> _patterns;

  explicit pattern_database() {}

 public:
  auto const& operator*() const noexcept { return _patterns; }
  auto const* operator->() const noexcept { return &_patterns; }
  auto const& operator[]( std::size_t const i ) const { return _patterns[i]; }

  static pattern entry( pattern_id const id ) {
    pattern_data empty;
    return std::make_unique<pattern_entry>( id, empty );
  }
};

struct pattern_type {
  static constexpr std::string_view REGEXP = "regexp";
  static constexpr std::string_view PURE = "pure";
  static constexpr std::string_view REGEXP_DOMAIN = "regexp-domain";
  static constexpr std::string_view PURE_DOMAIN = "pure-domain";
  static constexpr std::string_view IPADDR = "ipaddr";
};

using pattern = pattern_database::pattern;

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

  pattern pattern( precedence::type const precedence );
};

namespace parselet {

//--prefix-like-parselets-----------------------------------------------------

class prefix {
 public:
  virtual pattern accept( parser&, token const ) const = 0;
  virtual detail::precedence::type precedence() const noexcept = 0;
  virtual ~prefix() = default;
};

class pattern_id final : public prefix {
 private:
  pattern_id() {}

 public:
  pattern accept( parser& p, token const t ) const override {
    std::string lit{ p.slice_of( t ) };
    auto const id = std::stoull( lit );
    if( id > std::numeric_limits<pattern_database::pattern_id>::max() ) {
      throw std::runtime_error( "invalid pattern-id: pattern-id exceeds numerical limit" );
    }
    p.expect( token_type::COLON );
    return pattern_database::entry( static_cast<pattern_database::pattern_id>( id ) );
  }

  precedence::type precedence() const noexcept override { return precedence::DEFAULT; }

  static auto const& instance() noexcept {
    static pattern_id _self;
    return _self;
  }
};

//--infix-like-parselets------------------------------------------------------

class infix {
 public:
  virtual pattern accept( parser&, pattern, token const ) const = 0;
  virtual precedence::type precedence() const noexcept = 0;
  virtual ~infix() = default;
};

struct pattern_pure final : public infix {
 private:
  pattern_pure() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string pattern{ p.slice_of( t ) };
    pat->second["pattern"] = std::move( pattern );
    pat->second["pattern_type"] = "pure";
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_pure _self;
    return _self;
  }
};

struct pattern_regexp final : public infix {
 private:
  pattern_regexp() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string pattern{ p.slice_of( t ) };
    pat->second["pattern"] = std::move( pattern );
    pat->second["pattern_type"] = pattern_type::REGEXP;
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_regexp _self;
    return _self;
  }
};

struct pattern_ipaddr final : public infix {
 private:
  pattern_ipaddr() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string pattern{ p.slice_of( t ) };
    pat->second["pattern"] = std::move( pattern );
    pat->second["pattern_type"] = pattern_type::IPADDR;
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_ipaddr _self;
    return _self;
  }
};

struct pattern_regexp_domain final : public infix {
 private:
  pattern_regexp_domain() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string pattern{ p.slice_of( t ) };
    pat->second["pattern"] = std::move( pattern );
    pat->second["pattern_type"] = pattern_type::REGEXP_DOMAIN;
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_regexp_domain _self;
    return _self;
  }
};

struct pattern_domain final : public infix {
 private:
  pattern_domain() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string pattern{ p.slice_of( t ) };
    pat->second["pattern"] = std::move( pattern );
    pat->second["pattern_type"] = pattern_type::PURE_DOMAIN;
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_domain _self;
    return _self;
  }
};

struct pattern_annotation final : public infix {
 private:
  pattern_annotation() {}

 public:
  pattern accept( parser& p, pattern pat, token const t ) const override {
    std::string name{ p.slice_of( t ) };
    switch( p.peek().type() ) {
      default: pat->second[name] = "true"; break;
      case token_type::EQ: {
        p.pop();
        auto const value_token = p.peek();
        switch( value_token.type() ) {
          case token_type::ID:
          case token_type::NUM:
            p.pop();
            pat->second[name] = p.slice_of( value_token );
            break;
          default: throw std::runtime_error( "invalid pattern annotation" );
        }
        break;
      }
    }
    return pat;
  }

  precedence::type precedence() const noexcept override { return precedence::CALL; }

  static auto const& instance() noexcept {
    static pattern_annotation _self;
    return _self;
  }
};

class eos final : public infix {
 private:
  eos() {}

 public:
  pattern accept( parser&, pattern, token const ) const override {
    throw std::runtime_error( "eos-parselet: unexpected end of input" );
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
    case token_type::NUM: return parselet::pattern_id::instance();
    default: throw std::runtime_error( "parslet-prefix-select: unexpected token" );
  }
}

parselet::infix const& parselet_for_infixes( token const t ) {
  switch( t.type() ) {
    case token_type::BAR: return parselet::pattern_pure::instance();
    case token_type::SLASH: return parselet::pattern_regexp::instance();
    case token_type::TILDE: return parselet::pattern_regexp_domain::instance();
    case token_type::APOS: return parselet::pattern_domain::instance();
    case token_type::BACKSLASH: return parselet::pattern_ipaddr::instance();
    case token_type::ID: return parselet::pattern_annotation::instance();
    case token_type::EOS: return parselet::eos::instance();
    default: throw std::runtime_error( "parselet-infix-select: unexpected token" );
  }
}

} // namespace

//--pratt-parser-loop
inline pattern parser::pattern( precedence::type const precedence ) {
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

inline pattern_database const parse( std::istream& input ) {
  pattern_database patterns;
  std::string line;
  while( std::getline( input, line ) ) {
    if( line.empty() or line[0] == '#' ) { continue; }
    detail::parser parser{ line };
    auto pattern = parser.pattern( detail::precedence::DEFAULT );
    patterns._patterns.emplace_back( std::move( pattern ) );
  }
  return patterns;
}

inline pattern_database const parse( std::string_view const input_view ) {
  std::string input_string{ input_view };
  std::istringstream input{ input_string };
  return parse( input );
}

} // namespace
} // namespace ioc
