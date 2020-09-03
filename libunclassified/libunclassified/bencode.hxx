// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/bytestring.hxx>
#include <libunclassified/format-itoa.hxx>

#include <array>
#include <cstring>
#include <string_view>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified::bencode {

namespace format = unclassified::format;
using bytestring_view = unclassified::bytestring_view;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded( Ts... ) -> overloaded<Ts...>;

using byte_t = std::byte;

enum class type : unsigned int {
  number = 0xfff80000,
  bytestring,
  list,
  dict,
  // this must be last, order matters therefore
  end,
};

enum class error : unsigned int {
  invalid_number = 0xfff90000,
  unexpected_character,
  unexpected_key,
  truncated,
  invalid_bytestring,
  invalid_bytestring_length,
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

union token {
  byte_t _bytes[sizeof( double )];
  double _number;
  struct {
#if defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned int _payload;
    union {
      type _type;
      error _error;
    };
#elif defined( __BYTE_ORDER__ )
    union {
      type _type;
      error _error;
    };
    unsigned int _payload;
#endif
  };

  constexpr token( double const x ) noexcept : _number{ x } {}
  constexpr token( error const e ) noexcept : _error{ e } {}
  constexpr token( type const t ) noexcept : _payload{ 0 }, _type{ t } {}
  constexpr token( error const e, unsigned int const payload ) noexcept
    : _payload{ payload }, _error{ e } {}
  constexpr token( type const t, unsigned int const payload ) noexcept
    : _payload{ payload }, _type{ t } {}

  constexpr auto payload() const noexcept { return _payload; }
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

static_assert( sizeof( token ) == sizeof( double ) );

struct key {
  std::string_view const _key;
};

struct value {
  token _token;

  constexpr value( token const t ) noexcept : _token{ t } {}

  constexpr bool is_number() const noexcept { return _token._type <= type::number; }

  constexpr bool is_list() const noexcept { return _token._type == type::list; }

  constexpr bool is_dict() const noexcept { return _token._type == type::dict; }

  constexpr bool is_bytestring() const noexcept { return _token._type == type::bytestring; }

  constexpr bool is_end() const noexcept { return _token._type == type::end; }

  constexpr auto payload() const noexcept { return _token._payload; }

  constexpr bool is_error() const noexcept { return _token._type >= type::end; }

  constexpr std::int64_t to_int( std::int64_t const defv = 0ull ) const noexcept {
    return is_number() ? static_cast<std::int64_t>( _token._number ) : defv;
  }

  constexpr double to_number( double const defv = .0 ) const noexcept {
    return is_number() ? _token._number : defv;
  }

  constexpr error error_code() const noexcept { return _token._error; }
};

struct number final : public value {
  constexpr number( std::int64_t const v ) noexcept : value{ static_cast<double>( v ) } {}
};

struct bytestring final : public value {
  constexpr bytestring( unsigned int const length ) noexcept
    : value{ token{ type::bytestring, length } } {}
};

struct list final : public value {
  constexpr list( unsigned int const offset ) noexcept : value{ token{ type::list, offset } } {}
};

struct dict final : public value {
  constexpr dict( unsigned int offset ) noexcept : value{ token{ type::dict, offset } } {}
};

struct end final : public value {
  constexpr end( unsigned int const offset ) noexcept : value{ token{ type::end, offset } } {}
};

struct failed final : public value {
  constexpr failed( error const e ) noexcept : value{ e } {}
  constexpr failed( error const e, unsigned int const payload ) noexcept
    : value{ token{ e, payload } } {}
};

struct istream {
  byte_t const* _p;
  unsigned int const _size;
  unsigned int _cursor{ 0 };

  istream( byte_t const* p, unsigned int size ) : _p{ p }, _size{ size } {}

  istream( std::string_view const data )
    : _p{ reinterpret_cast<byte_t const*>( data.data() ) },
      _size{ static_cast<unsigned int>( data.size() ) } {}

  template <std::size_t N>
  istream( std::array<std::byte, N> const& data )
    : _p{ data.data() }, _size{ static_cast<unsigned int>( data.size() ) } {}

  template <unsigned int N>
  istream( byte_t ( &data )[N] ) : _p{ data }, _size{ N } {}

  inline int peek() const noexcept {
    return ( _cursor >= _size ) ? -1 : static_cast<int>( _p[_cursor] );
  }

  inline void operator++() noexcept { _cursor = std::min( ++_cursor, _size ); }

  inline int pop() noexcept {
    auto c = peek();
    operator++();
    return c;
  }

  inline auto cursor() const noexcept { return _cursor; }

  inline bool eos() const noexcept { return _cursor >= _size; }

  inline unsigned int available() const noexcept {
    if( _cursor >= _size ) { return 0u; }
    return _size - _cursor;
  }

  // slice a bytestring ( at `_cursor+1` ). we use this hack such that if the
  // user does not consume the bytestring the bencod'er fails parsing the next
  // character which happens to be ':'.
  inline std::string_view const slice( unsigned int length ) noexcept {
    if( available() < ( length + 1 ) ) { return { nullptr, 0 }; }
    auto p = _p + _cursor + 1;
    _cursor += length + 1;
    return { reinterpret_cast<char const*>( p ), length };
  }

  // slice a bytestring ( at `_cursor+1` )
  inline bytestring_view const slice_bs( unsigned int length ) noexcept {
    if( available() < ( length + 1 ) ) { return bytestring_view{ nullptr, 0 }; }
    auto p = _p + _cursor + 1;
    _cursor += length + 1;
    return bytestring_view{ p, length };
  }

  inline void skip( unsigned int length ) noexcept {
    if( available() < length ) {
      _cursor = _size;
    } else {
      _cursor += length;
    }
  }
};

inline std::uint64_t parse_number( istream& is ) noexcept {
  std::uint64_t v = 0;
  while( true ) {
    if( auto c = is.peek(); c >= '0' && c <= '9' ) {
      v = ( v * 10 ) + static_cast<std::uint64_t>( c - '0' );
      ++is;
    } else {
      break;
    }
  }
  return v;
}

template <type T, typename V>
inline void expect( istream& is, V&& v ) {
  if constexpr( T == type::bytestring ) {
    switch( is.peek() ) {
      case '0' ... '9': {
        std::uint64_t len = parse_number( is );
        if( ( len >> 32 ) > 0 || ( len + 1 > is.available() ) ) {
          v( failed{ error::invalid_bytestring_length } );
          return;
        }
        if( is.peek() != ':' ) {
          v( failed{ error::invalid_bytestring } );
          return;
        }
        v( bytestring{ static_cast<unsigned int>( len ) } );
        return;
      }
      case -1: v( failed{ error::truncated } ); return;
      default: v( failed{ error::unexpected_character } ); return;
    }
  } else if constexpr( T == type::number ) {
    switch( is.peek() ) {
      case 'i': {
        ++is;
        std::int64_t const sign = is.peek() == '-' ? static_cast<void>( ++is ), -1 : 1;
        auto digits_start = is.cursor();
        std::uint64_t x = parse_number( is );
        if( ( x >> 32 ) > static_cast<unsigned int>( type::number ) ||
            ( digits_start == is.cursor() ) ) {
          return v( failed{ error::invalid_number, digits_start } );
        }
        switch( is.peek() ) {
          case 'e':
            ++is;
            v( number{ static_cast<std::int64_t>( x ) * sign } );
            return;
          case -1: v( failed{ error::truncated } ); return;
          default: v( failed{ error::unexpected_character } ); return;
        }
        break;
      }
      case -1: v( failed{ error::truncated } ); return;
      default: v( failed{ error::unexpected_character } ); return;
    }
  } else if constexpr( T == type::dict ) {
    switch( is.peek() ) {
      case 'd':
        ++is;
        v( dict{ is.cursor() } );
        return;
      case -1: v( failed{ error::truncated } ); return;
      default: v( failed{ error::unexpected_character } ); return;
    }
  } else if constexpr( T == type::list ) {
    switch( is.peek() ) {
      case 'l':
        ++is;
        v( list{ is.cursor() } );
        return;
      case -1: v( failed{ error::truncated } ); return;
      default: v( failed{ error::unexpected_character } ); return;
    }
  } else if constexpr( T == type::end ) {
    switch( is.peek() ) {
      case 'e':
        ++is;
        v( end{ is.cursor() } );
        return;
      case -1: v( failed{ error::truncated } ); return;
      default: v( failed{ error::unexpected_character } ); return;
    }
  }
}

template <typename V>
inline void parse1( istream& is, V&& v ) noexcept {
  // clang-format off
  switch( is.peek() )
  {
    case 'i': expect<type::number>( is, std::forward<V>( v ) ); break;
    case '0' ... '9': expect<type::bytestring>( is, std::forward<V>( v ) ); break;
    case 'l': ++is; v( list{ is.cursor() } ); break;
    case 'd': ++is; v( dict{ is.cursor() } ); break;
    case 'e': ++is; v( end{ is.cursor() } );  break;
    default: v( failed{ error::unexpected_character } ); break;
  }
  // clang-format on
}

template <typename V>
inline void parse( istream& is, V&& v ) noexcept {
  bool done = false;
  while( ! done && ! is.eos() ) {
    parse1( is, [&]( auto&& x ) {
      using T = std::decay_t<decltype( x )>;
      if constexpr( std::is_same_v<T, failed> ) { done = true; }
      v( std::forward<T>( x ) );
    } );
  }
}

template <typename V>
inline void parse( std::string_view const data, V&& v ) noexcept {
  auto is = istream{ data };
  parse( is, std::forward<V>( v ) );
}

struct dstream : public istream {
  value _error{ error::truncated };
  bool _failed{ false };

  template <typename... Args>
  dstream( Args&&... args ) : istream{ std::forward<Args>( args )... } {}

  template <typename V>
  inline void operator()( V const v ) {
    if constexpr( std::is_same_v<V, failed> ) {
      _error = v;
      _failed = true;
    }
  }

  template <type V, typename Cont>
  inline dstream& then( Cont&& cont ) noexcept {
    if( _failed ) { return *this; }
    expect<V>( *this, [&]( auto&& v ) {
      using T = std::decay_t<decltype( v )>;
      if constexpr( std::is_same_v<T, failed> ) {
        _error = v;
        _failed = true;
      } else {
        cont( std::forward<T>( v ) );
      }
    } );
    return *this;
  }

  inline dstream& dict() noexcept {
    if( _failed ) { return *this; }
    expect<type::dict>( *this, *this );
    return *this;
  }

  inline dstream& end() noexcept {
    if( _failed ) { return *this; }
    expect<type::end>( *this, *this );
    return *this;
  }
};

inline dstream& operator>>( dstream& is, std::string_view& out ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) { out = is.slice( v.payload() ); },
      } );

  return is;
}

inline dstream& operator>>( dstream& is, bytestring_view& out ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) { out = is.slice_bs( v.payload() ); },
      } );

  return is;
}

template <std::size_t N>
inline dstream& operator>>( dstream& is, std::byte ( &data )[N] ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) {
            if( v.payload() != N ) {
              is( failed{ error::invalid_bytestring_length } );
              return;
            }
            auto const key = is.slice_bs( v.payload() );
            std::memcpy( data, key.data(), N );
          },
      } );
  return is;
}

template <std::size_t N>
inline dstream& operator>>( dstream& is, std::array<std::byte, N>& data ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) {
            if( v.payload() != N ) {
              is( failed{ error::invalid_bytestring_length } );
              return;
            }
            auto const key = is.slice_bs( v.payload() );
            std::memcpy( data.data(), key.data(), N );
          },
      } );
  return is;
}

inline dstream& operator>>( dstream& is, struct key const expected_key ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) {
            auto const key = is.slice( v.payload() );
            if( key not_eq expected_key._key ) { is( failed{ error::unexpected_key } ); }
          },
      } );
  return is;
}

inline dstream& operator>>( dstream& is, std::int64_t& out ) noexcept {
  if( is._failed ) { return is; }
  expect<type::number>( is,
                        overloaded{
                            [&]( failed const v ) { is( v ); },
                            [&]( number const v ) { out = v.to_int(); },
                        } );
  return is;
}

inline dstream& operator>>( dstream& is, struct dict const ) noexcept {
  if( is._failed ) { return is; }
  expect<type::dict>( is, is );
  return is;
}

inline dstream& operator>>( dstream& is, struct end const ) noexcept {
  if( is._failed ) { return is; }
  expect<type::end>( is, is );
  return is;
}

inline dstream& operator>>( dstream& is, struct list const ) noexcept {
  if( is._failed ) { return is; }
  expect<type::list>( is, is );
  return is;
}

template <typename T,
          std::enable_if_t<std::is_integral<T>::value && sizeof( T ) <= sizeof( std::int64_t ), int> = 0>
inline dstream& operator>>( dstream& is, T& out ) noexcept {
  if( is._failed ) { return is; }
  expect<type::number>( is,
                        overloaded{
                            [&]( failed const v ) { is( v ); },
                            [&]( number const v ) { out = static_cast<T>( v.to_int() ); },
                        } );
  return is;
}

struct ostream {
  byte_t* _p;
  unsigned int const _size;
  unsigned int _cursor{ 0 };
  bool _overflow{ false };

  template <unsigned int N>
  ostream( byte_t ( &data )[N] ) : _p{ data }, _size{ N } {}

  template <unsigned int N>
  ostream( char ( &data )[N] ) : _p{ reinterpret_cast<byte_t*>( data ) }, _size{ N } {}

  ostream( byte_t* const p, unsigned int const n ) : _p{ p }, _size{ n } {}

  ostream( char* const p, unsigned int const n ) : _p{ reinterpret_cast<byte_t*>( p ) }, _size{ n } {}

  inline void rewind() noexcept {
    _cursor = 0;
    _overflow = false;
  }

  inline auto cursor() const noexcept { return _cursor; }

  inline auto available() const noexcept { return _size - _cursor; }

  inline byte_t const* current_ptr() const noexcept { return _p + _cursor; }

  inline bytestring_view const slice( unsigned int const offset,
                                      unsigned int const len ) const noexcept {
    if( ( ( offset + len ) < len ) || offset + len > _size ) { return bytestring_view{ nullptr, 0 }; }
    return bytestring_view{ _p + offset, len };
  }

  inline void push( std::int64_t const x ) noexcept {
    auto digits = format::ilog10( x );
    digits += x < 0;
    if( available() < digits ) {
      _overflow = true;
      return;
    }
    format::itoa( reinterpret_cast<char*>( _p + _cursor ), digits, x );
    if( x < 0 ) { _p[_cursor] = byte_t( '-' ); }
    _cursor += digits;
  }

  inline void push( byte_t const* begin, std::size_t const len ) noexcept {
    if( available() < len ) {
      _overflow = true;
      return;
    }
    std::memcpy( _p + _cursor, begin, len );
    _cursor += len;
  }

  inline void push( char const* begin, std::size_t const len ) noexcept {
    if( available() < len ) {
      _overflow = true;
      return;
    }
    std::memcpy( _p + _cursor, begin, len );
    _cursor += len;
  }

  inline void push( bytestring_view const view ) noexcept { push( view.data(), view.size() ); }

  inline void push( char const c ) noexcept {
    if( available() < 1 ) {
      _overflow = true;
      return;
    }
    _p[_cursor] = byte_t( c );
    _cursor++;
  }
};

inline dstream& operator>>( dstream& is, ostream& out ) noexcept {
  if( is._failed ) { return is; }
  expect<type::bytestring>( //
      is,
      overloaded{
          [&]( failed const v ) { is( v ); },
          [&]( bytestring const v ) { out.push( is.slice_bs( v.payload() ) ); },
      } );

  return is;
}

struct build {
  ostream _os;

  template <typename C, unsigned int N>
  build( C ( &data )[N] ) : _os{ data } {}

  template <typename C, std::size_t N>
  build( std::array<C, N>& data ) : _os{ data.data(), N } {}

  struct bytes {
    std::byte const* const _data;
    std::size_t const _size;
    constexpr bytes( std::byte const* const cbegin, std::byte const* const cend ) noexcept
      : _data{ cbegin }, _size{ static_cast<std::size_t>( cend - cbegin ) } {}
    constexpr bytes( std::byte const* const data, std::size_t const size ) noexcept
      : _data{ data }, _size{ size } {}
  };

  using key = key;
  static constexpr dict dict{ 0 };
  static constexpr list list{ 0 };
  static constexpr end end{ 0 };

  inline unsigned int used() const noexcept { return _os.cursor(); }

  inline bool success() const noexcept { return not _os._overflow; }
};

inline build& operator<<( build& b, std::int64_t const n ) noexcept {
  b._os.push( 'i' );
  b._os.push( n );
  b._os.push( 'e' );
  return b;
}

template <typename T,
          std::enable_if_t<std::is_integral<T>::value && sizeof( T ) <= sizeof( std::int64_t ), int> = 0>
inline build& operator<<( build& b, T const x ) {
  b._os.push( 'i' );
  b._os.push( static_cast<std::int64_t>( x ) );
  b._os.push( 'e' );
  return b;
}

inline build& operator<<( build& b, build::key const k ) noexcept {
  b._os.push( static_cast<std::int64_t>( k._key.size() ) );
  b._os.push( ':' );
  b._os.push( k._key.data(), k._key.size() );
  return b;
}

inline build& operator<<( build& b, struct list const ) noexcept {
  b._os.push( 'l' );
  return b;
}

inline build& operator<<( build& b, struct dict const ) noexcept {
  b._os.push( 'd' );
  return b;
}

inline build& operator<<( build& b, struct end const ) noexcept {
  b._os.push( 'e' );
  return b;
}

template <std::size_t N>
inline build& operator<<( build& b, byte_t const ( &data )[N] ) noexcept {
  b._os.push( static_cast<std::int64_t>( N ) );
  b._os.push( ':' );
  b._os.push( data, N );
  return b;
}

inline build& operator<<( build& b, build::bytes const bytes ) noexcept {
  b._os.push( static_cast<std::int64_t>( bytes._size ) );
  b._os.push( ':' );
  b._os.push( bytes._data, bytes._size );
  return b;
}

inline build& operator<<( build& b, bytestring_view const bytes ) noexcept {
  b._os.push( static_cast<std::int64_t>( bytes.size() ) );
  b._os.push( ':' );
  b._os.push( bytes.data(), bytes.size() );
  return b;
}

inline build& operator<<( build& b, std::string_view const s ) noexcept {
  b._os.push( static_cast<std::int64_t>( s.size() ) );
  b._os.push( ':' );
  b._os.push( s.data(), s.size() );
  return b;
}

} // namespace unclassified::bencode

#ifdef __clang__
#pragma clang diagnostic pop
#endif
