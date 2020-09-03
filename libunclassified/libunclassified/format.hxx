// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/format-dtoa.hxx>
#include <libunclassified/format-itoa.hxx>
#include <libunclassified/format-timestamp.hxx>

#include <string_view>

namespace unclassified::format {
namespace detail {

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct hex {
  T const _value;
};

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct hexu {
  T const _value;
};

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct oct {
  T const _value;
};

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct dec {
  T const _value;
};

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
struct chr {
  T const _value;
};

struct ts {
  static constexpr auto FORMAT = timestamp_flags::yyyy_mm_dd | timestamp_flags::msecs |
      timestamp_flags::sep3;
  unsigned long long _value;
};

template <std::size_t Size>
struct stream {
  using char_t = char;
  char_t* _data;
  std::size_t _offset;

  template <typename T>
  constexpr void encode( hex<T> const v ) noexcept {
    auto x = v._value;
    auto digits = ilog16( x );
    digits += x < 0;
    digits += 2;
    if( available() < digits ) { return; }
    itoa_base16l( _data + _offset, digits, x );
    if( x < 0 ) {
      _data[_offset + 2] = 'x';
      _data[_offset + 1] = '0';
      _data[_offset + 0] = '-';
    } else {
      _data[_offset + 1] = 'x';
      _data[_offset + 0] = '0';
    }
    _offset += digits;
  }

  template <typename T>
  constexpr void encode( oct<T> const v ) noexcept {
    auto x = v._value;
    auto digits = ilog8( x );
    digits += x < 0;
    digits += 1;
    if( available() < digits ) { return; }
    itoa_base8( _data + _offset, digits, x );
    if( x < 0 ) {
      _data[_offset + 1] = '0';
      _data[_offset + 0] = '-';
    } else {
      _data[_offset + 0] = '0';
    }
    _offset += digits;
  }

  template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
  constexpr void encode( T const x ) noexcept {
    auto digits = ilog10( x );
    digits += x < 0;
    if( available() < digits ) { return; }
    itoa( _data + _offset, digits, x );
    if( x < 0 ) { _data[_offset] = '-'; }
    _offset += digits;
  }

  template <typename T>
  constexpr void encode( dec<T> const v ) noexcept {
    auto x = v._value;
    // explicitly using `T` is important
    // in case `T` is same as `char`
    encode<T>( x );
  }

  constexpr void encode( char const x ) noexcept {
    if( available() < 1 ) return;
    _data[_offset] = x;
    _offset++;
  }

  template <typename T>
  constexpr void encode( chr<T> const v ) noexcept {
    encode( static_cast<char>( v._value ) );
  }

  inline void encode( double const v ) noexcept {
    if( available() < 512 ) {
      char_t buf[512];
      auto digits = dtoa( v, buf );
      if( available() < digits ) return;
      std::memcpy( _data + _offset, buf, digits );
      _offset += digits;
    } else {
      auto digits = dtoa( v, _data + _offset );
      _offset += digits;
    }
  }

  constexpr void encode( ts const t ) noexcept {
    if( available() < TIMESTAMP_BUFSZ ) return;
    auto const digits = detail::format_ts_impl( _data + _offset, t._value, ts::FORMAT );
    _offset += digits;
  }

  constexpr void encode( std::string_view const view ) noexcept {
    if( available() < view.size() ) { return; }
    std::memcpy( _data + _offset, view.data(), view.size() );
    _offset += view.size();
  }

  template <std::size_t N>
  constexpr void encode( char const ( &data )[N] ) noexcept {
    static_assert( N > 0 );
    auto n = data[N - 1] == '\0' ? N - 1 : N;
    if( available() < n ) { return; }
    std::memcpy( _data + _offset, data, n );
    _offset += n;
  }

  constexpr std::size_t available() const noexcept { return Size - _offset; }

  constexpr std::string_view view() const noexcept { return { _data, _offset }; }

  template <typename X>
  constexpr stream& operator<<( X&& x ) noexcept {
    encode( std::forward<X>( x ) );
    return *this;
  }
};

} // namespace detail

template <std::size_t Size>
detail::stream<Size> stream( char ( &data )[Size] ) {
  return detail::stream<Size>{ ._data = data, ._offset = 0 };
}

template <std::size_t Size>
detail::stream<Size> stream( std::array<char, Size>& data ) {
  return detail::stream<Size>{ ._data = data.data(), ._offset = 0 };
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
constexpr detail::hex<T> hex( T const value ) noexcept {
  return { value };
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
constexpr detail::hexu<T> hexu( T const value ) noexcept {
  return { value };
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
constexpr detail::oct<T> oct( T const value ) noexcept {
  return { value };
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
constexpr detail::dec<T> dec( T const value ) noexcept {
  return { value };
}

template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
constexpr detail::chr<T> chr( T const value ) noexcept {
  return { value };
}

constexpr detail::ts ts( unsigned long long const value ) noexcept { return { value }; }

template <typename Stream, typename... Args>
constexpr void encode( Stream& os, Args&&... args ) noexcept {
  ( os << ... << args );
}

} // namespace unclassified::format
