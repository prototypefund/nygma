// License: RoundRobin-1.0.0

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <vector>

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wold-style-cast"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wsign-conversion"
#  pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#  pragma clang diagnostic ignored "-Wcast-qual"
#  pragma clang diagnostic ignored "-Wswitch-enum"
#endif

extern "C" {
#include <yxml/yxml.h>
}

#ifdef __clang__
#  pragma clang diagnostic pop
#endif

namespace owl::xml {
struct parser {
  static constexpr std::size_t STACK_SIZE = 8096;
  static constexpr auto EOS = std::ifstream::traits_type::eof();
  std::unique_ptr<char[]> _stack;
  yxml_t _state;
  std::ifstream& _is;
  yxml_ret_t _cached_token = YXML_EEOF;

  parser( std::ifstream& is ) : _stack{ std::make_unique<char[]>( STACK_SIZE ) }, _is{ is } {
    yxml_init( &_state, _stack.get(), STACK_SIZE );
  }

  auto cached_token() const noexcept { return _cached_token; }

  auto advance() noexcept {
    auto c = EOS;
    auto token = YXML_EEOF;
    do {
      c = _is.get();
      if( c == EOS ) { break; }
      token = yxml_parse( &_state, c );
    } while( c != EOS and token == YXML_OK );
    _cached_token = token;
    return _cached_token;
  }

  bool expect( yxml_ret_t const token ) noexcept { return _cached_token == token; }

  void skip( yxml_ret_t const token ) noexcept {
    while( expect( token ) ) { advance(); }
  }

  void skip( yxml_ret_t const a, yxml_ret_t const b ) noexcept {
    while( cached_token() == a or cached_token() == b ) { advance(); }
  }

  std::string_view data() const noexcept { return _state.data; }

  std::string_view attr() noexcept { return { _state.attr, yxml_symlen( &_state, _state.attr ) }; }

  std::string_view elem() noexcept {
    if( _cached_token != YXML_ELEMSTART ) {
      return "";
    } else {
      return { _state.elem, yxml_symlen( &_state, _state.elem ) };
    }
  }
};
} // namespace owl::xml

namespace owl::pdml {

struct node {
  enum class type { FIELD, PROTO };
  type const _type;
  unsigned const _level;
  std::string _text;

  node( type const type, unsigned const lvl ) : _type{ type }, _level{ lvl } {}

  template <typename Visitor>
  void accept( Visitor&& v ) const noexcept;
};

struct field : node {
  field( unsigned const lvl ) : node{ node::type::FIELD, lvl } {}
  std::string const& summary() { return _text; }
};

struct proto : node {
  proto( unsigned const lvl ) : node{ node::type::PROTO, lvl } {}
  std::string const& summary() { return _text; }
};

template <typename Visitor>
void node::accept( Visitor&& v ) const noexcept {
  switch( _type ) {
    case type::FIELD: v( static_cast<field const&>( *this ) ); break;
    case type::PROTO: v( static_cast<proto const&>( *this ) ); break;
  }
}

struct packet {
  static constexpr auto MAX_SUMMARY_DEPTH = 3;
  std::vector<std::string> _summary;
  std::vector<std::unique_ptr<node>> _nodes;

  void summary( std::size_t const i ) {
    struct slurp {
      std::vector<std::string>& _summary;
      slurp( std::vector<std::string>& s ) : _summary{ s } {}
      void operator()( field const& ) const noexcept {}
      void operator()( proto const& p ) noexcept {
        if( _summary.size() < MAX_SUMMARY_DEPTH ) { _summary.push_back( p._text ); }
      }
    };
    if( _summary.empty() ) {
      std::ostringstream os;
      os << "#( " << std::setw( 4 ) << i << " )";
      _summary.emplace_back( os.str() );
      slurp visitor{ _summary };
      auto it = _nodes.rbegin();
      for( ; it != _nodes.rend() and visitor._summary.size() < MAX_SUMMARY_DEPTH; ++it ) {
        ( *it )->accept( visitor );
      }
      for( auto j = MAX_SUMMARY_DEPTH - _summary.size(); j > 0; --j ) {
        _summary.emplace_back( " ... " );
      }
    }
  }

  std::vector<std::string> const& summary() const noexcept { return _summary; }
};

namespace {

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wswitch-enum"
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define __ERROR__ ( -( __LINE__ ) )

void parse_pdml( xml::parser& p, std::vector<packet>& packets ) {
  struct state {
    using type = int;
    enum : type {
      START,
      IN_PDML_ELEM,
      IN_PDML_ATTR,
      PACKETS,
      PACKET_ELEM_ENTRY,
      PACKET_ELEM_ENTRY_ATTR,
    };
  };
  state::type state = state::START;
  std::string show_attribute;
  //std::vector<std::pair<node::type, std::string>> stack;

  auto attribute = [&packets]() -> auto& { return packets.back()._nodes.back()->_text; };

  unsigned lvl = 0;
  while( true ) {
    switch( state ) {
      case state::START: {
        switch( p.cached_token() ) {
          case YXML_ELEMSTART:
            state = p.elem() == "pdml" ? state::IN_PDML_ELEM : __ERROR__;
            p.advance();
            continue;
          default: state = __ERROR__; continue;
        }
      }

        //--process-<pdml ...>------------------------------------------------

      case state::IN_PDML_ELEM: {
        switch( p.cached_token() ) {
          case YXML_ATTRSTART:
            p.advance();
            state = state::IN_PDML_ATTR;
            continue;
          case YXML_CONTENT:
            p.advance();
            p.skip( YXML_CONTENT );
            state = state::IN_PDML_ELEM;
            continue;
          case YXML_ELEMSTART: //--process-first-packet-----------------------
            state = state::PACKETS;
            continue;
          default: state = __ERROR__; continue;
        }
      }
      case state::IN_PDML_ATTR: {
        switch( p.cached_token() ) {
          case YXML_ATTRVAL:
            p.advance();
            p.skip( YXML_ATTRVAL );
            state = state::IN_PDML_ATTR;
            continue;
          case YXML_ATTREND:
            p.advance();
            state = state::IN_PDML_ELEM;
            continue;
          default: state = __ERROR__; continue;
        }
      }

        //--process-packets---------------------------------------------------

      case state::PACKETS: {
        switch( p.cached_token() ) {
          case YXML_ELEMSTART:
            state = p.elem() == "packet" ? state::PACKET_ELEM_ENTRY : __ERROR__;
            if( state < 0 ) { std::clog << __LINE__ << "e = " << p.elem() << std::endl; }
            p.advance();
            packets.emplace_back();
            lvl = 0;
            continue;
          case YXML_CONTENT:
            p.advance();
            p.skip( YXML_CONTENT );
            state = state::PACKETS;
            continue;
          case YXML_ELEMEND:
            // this is actually the `pdml` end tag
            p.advance();
            return;
          default: state = __ERROR__; continue;
        }
      }

      case state::PACKET_ELEM_ENTRY: {
        switch( p.cached_token() ) {
          case YXML_ATTRSTART:
            if( p.attr() == "show" ) {
              if( attribute().size() > 0 ) {
                p.advance();
                continue;
              }
              state = state::PACKET_ELEM_ENTRY_ATTR;
            } else if( p.attr() == "showname" ) {
              attribute().clear();
              state = state::PACKET_ELEM_ENTRY_ATTR;
            }
            p.advance();
            continue;
          case YXML_ATTRVAL:
            p.advance();
            p.skip( YXML_ATTRVAL );
            state = state::PACKET_ELEM_ENTRY;
            continue;
          case YXML_ATTREND:
            p.advance();
            state = state::PACKET_ELEM_ENTRY;
            continue;
          case YXML_ELEMSTART:
            if( p.elem() == "proto" ) {
              packets.back()._nodes.emplace_back( std::make_unique<proto>( lvl ) );
              state = state::PACKET_ELEM_ENTRY;
            } else if( p.elem() == "field" ) {
              packets.back()._nodes.emplace_back( std::make_unique<field>( lvl ) );
              state = state::PACKET_ELEM_ENTRY;
            } else {
              state = __ERROR__;
            }
            lvl++;
            show_attribute.clear();
            p.advance();
            continue;
          case YXML_ELEMEND:
            state = lvl == 0 ? state::PACKETS : state::PACKET_ELEM_ENTRY;
            lvl = std::max( 0u, lvl - 1 );
            p.advance();
            continue;
          case YXML_CONTENT:
            p.advance();
            p.skip( YXML_CONTENT );
            state = state::PACKET_ELEM_ENTRY;
            continue;
          default: state = __ERROR__; continue;
        }
      }

      case state::PACKET_ELEM_ENTRY_ATTR: {
        switch( p.cached_token() ) {
          case YXML_ATTRVAL:
            attribute() += p.data();
            p.advance();
            continue;
          case YXML_ATTREND:
            state = state::PACKET_ELEM_ENTRY;
            p.advance();
            continue;
          default: state = __ERROR__; continue;
        }
      }

      default:
        std::clog << __LINE__ << ": state = " << state
                  << " token = " << static_cast<int>( p.cached_token() ) << " lvl = " << lvl
                  << std::endl;
        return;
    }
  }
}

std::vector<packet> parse2( xml::parser& p ) {
  std::vector<packet> packets;
  p.advance();
  while( p.expect( YXML_PISTART ) ) {
    p.advance();
    p.skip( YXML_PICONTENT );
    if( not p.expect( YXML_PIEND ) ) {
      std::clog << __LINE__ << ": t = " << static_cast<int>( p.cached_token() ) << std::endl;
      return packets;
    }
    p.advance();
  }
  parse_pdml( p, packets );
  return packets;
}

inline auto parse( std::filesystem::path const& path ) {
  std::ifstream is{ path };
  xml::parser p{ is };
  return parse2( p );
}

inline auto parse( std::ifstream& is ) {
  xml::parser p{ is };
  return parse2( p );
}

#ifdef __clang__
#  pragma clang diagnostic pop
#endif

} // namespace

} // namespace owl::pdml
