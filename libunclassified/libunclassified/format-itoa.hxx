// SPDX-License-Identifier: 0BSD

// @see <https://github.com/h2o/qrintf>
//
// Copyright (c) 2020 mb.
// Copyright (c) 2014,2015 DeNA Co., Ltd., Masahiro, Ide, Kazuho Oku.

#pragma once

#include <climits>
#include <cstdlib>
#include <type_traits>

// TODO: remove code duplication ( e.g. itoa_base16l & itoa_base16u )

namespace unclassified::format {

using char_t = char;

namespace detail {
static constexpr char_t const digits_table[] = {
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899" };

static constexpr char_t const digits_table_base8[] = {
    "0001020304050607"
    "1011121314151617"
    "2021222324252627"
    "3031323334353637"
    "4041424344454647"
    "5051525354555657"
    "6061626364656667"
    "7071727374757677" };

static constexpr char_t const digits_table_base16l[] = {
    "000102030405060708090a0b0c0d0e0f"
    "101112131415161718191a1b1c1d1e1f"
    "202122232425262728292a2b2c2d2e2f"
    "303132333435363738393a3b3c3d3e3f"
    "404142434445464748494a4b4c4d4e4f"
    "505152535455565758595a5b5c5d5e5f"
    "606162636465666768696a6b6c6d6e6f"
    "707172737475767778797a7b7c7d7e7f"
    "808182838485868788898a8b8c8d8e8f"
    "909192939495969798999a9b9c9d9e9f"
    "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
    "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
    "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
    "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
    "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff" };

static constexpr char_t const digits_table_base16u[] = {
    "000102030405060708090A0B0C0D0E0F"
    "101112131415161718191A1B1C1D1E1F"
    "202122232425262728292A2B2C2D2E2F"
    "303132333435363738393A3B3C3D3E3F"
    "404142434445464748494A4B4C4D4E4F"
    "505152535455565758595A5B5C5D5E5F"
    "606162636465666768696A6B6C6D6E6F"
    "707172737475767778797A7B7C7D7E7F"
    "808182838485868788898A8B8C8D8E8F"
    "909192939495969798999A9B9C9D9E9F"
    "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
    "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
    "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
    "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
    "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
    "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF" };

/* @see: <http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10> */
inline unsigned ilog10u32( unsigned const v ) noexcept {
  static_assert( sizeof( unsigned ) == 4 );
#define LOG2( N )                                                                                     \
  ( static_cast<unsigned>( ( sizeof( unsigned ) * 8 ) -                                               \
                           static_cast<unsigned>( __builtin_clz( (N)-1 ) ) ) )
  static constexpr unsigned long ilog10_table[] = {
      1UL,       10UL,       100UL,       1000UL,       10000UL,  100000UL,
      1000000UL, 10000000UL, 100000000UL, 1000000000UL, UINT_MAX,
  };
  if( v != 0 ) {
    unsigned t;
    t = ( ( LOG2( v ) + 1 ) * 1233 ) / 4096;
    return t + ( v >= ilog10_table[t] );
  } else {
    return 1u;
  }
#undef LOG2
}

static inline unsigned ilog10u64( unsigned long long const v ) noexcept {
  static_assert( sizeof( unsigned long long ) == 8 );
#define LOG2( N )                                                                                     \
  ( static_cast<unsigned>( ( sizeof( long long ) * 8 ) -                                              \
                           static_cast<unsigned>( __builtin_clzll( (N)-1 ) ) ) )
  static constexpr unsigned long long ilog10_table[] = {
      1ULL,
      10ULL,
      100ULL,
      1000ULL,
      10000ULL,
      100000ULL,
      1000000ULL,
      10000000ULL,
      100000000ULL,
      1000000000ULL,
      10000000000ULL,
      100000000000ULL,
      1000000000000ULL,
      10000000000000ULL,
      100000000000000ULL,
      1000000000000000ULL,
      10000000000000000ULL,
      100000000000000000ULL,
      1000000000000000000ULL,
      10000000000000000000ULL,
      ULLONG_MAX,
  };
  if( v != 0 ) {
    unsigned t;
    t = ( ( LOG2( v ) + 1 ) * 1233 ) / 4096;
    return t + ( v >= ilog10_table[t] );
  } else {
    return 1;
  }
#undef LOG2
}

template <typename T, typename std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline unsigned ilog10u( T const v ) noexcept {
  if constexpr( sizeof( T ) <= 4 ) {
    return ilog10u32( v );
  } else if constexpr( sizeof( T ) == 8 ) {
    return ilog10u64( v );
  }
}

template <typename T, typename std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline unsigned _clz( T const v ) noexcept {
  if constexpr( sizeof( T ) <= sizeof( unsigned ) ) {
    return static_cast<unsigned>( __builtin_clz( v ) );
  } else if constexpr( sizeof( T ) == sizeof( unsigned long long ) ) {
    return static_cast<unsigned>( __builtin_clzll( v ) );
  } else if constexpr( sizeof( T ) == sizeof( unsigned long ) ) {
    return static_cast<unsigned>( __builtin_clzl( v ) );
  }
}

template <typename T, typename std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline unsigned ilog16u( T const v ) noexcept {
  if( v != 0 ) {
    unsigned bits = sizeof( T ) * 8 - _clz<T>( v );
    return ( bits + 3 ) >> 2;
  } else {
    return 1;
  }
}

template <typename T, typename std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline unsigned ilog8u( T const v ) noexcept {
  if( v != 0 ) {
    unsigned bits = sizeof( T ) * 16 - 2 * _clz<T>( v );
    return ( bits + 6 ) >> 3;
  } else {
    return 1;
  }
}

} // namespace detail

template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
inline unsigned ilog10( T const v ) noexcept {
  if constexpr( std::is_signed<T>::value ) {
    if( v < 0 ) {
      return detail::ilog10u( typename std::make_unsigned<T>::type( -v ) );
    } else {
      return detail::ilog10u( typename std::make_unsigned<T>::type( v ) );
    }
  } else if constexpr( std::is_unsigned<T>::value ) {
    return detail::ilog10u( v );
  }
}

template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
inline unsigned ilog16( T const v ) noexcept {
  if constexpr( std::is_signed<T>::value ) {
    if( v < 0 ) {
      return detail::ilog16u( typename std::make_unsigned<T>::type( -v ) );
    } else {
      return detail::ilog16u( typename std::make_unsigned<T>::type( v ) );
    }
  } else if constexpr( std::is_unsigned<T>::value ) {
    return detail::ilog16u( v );
  }
}

template <typename T, typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
inline unsigned ilog8( T const v ) noexcept {
  if constexpr( std::is_signed<T>::value ) {
    if( v < 0 ) {
      return detail::ilog8u( typename std::make_unsigned<T>::type( -v ) );
    } else {
      return detail::ilog8u( typename std::make_unsigned<T>::type( v ) );
    }
  } else if constexpr( std::is_unsigned<T>::value ) {
    return detail::ilog8u( v );
  }
}

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline void itoa( char_t* const out, unsigned const digits, T const x ) noexcept {
  auto v = x;
  char_t* p = out + digits;
  while( v >= 100 ) {
    unsigned idx = v % 100 * 2;
    *--p = detail::digits_table[idx + 1];
    *--p = detail::digits_table[idx];
    v /= 100;
  }
  if( v < 10 ) {
    *--p = '0' + static_cast<char_t>( v );
  } else {
    *--p = detail::digits_table[v * 2 + 1];
    *--p = detail::digits_table[v * 2];
  }
}

template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
inline void itoa( char_t* const out, unsigned const digits, T const v ) noexcept {
  if( v < 0 ) {
    itoa( out, digits, typename std::make_unsigned<T>::type( -v ) );
  } else {
    itoa( out, digits, typename std::make_unsigned<T>::type( v ) );
  }
}

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline void itoa_base8( char_t* const out, unsigned const digits, T const x ) noexcept {
  auto v = x;
  char_t* p = out + digits;
  while( v >= 0100 ) {
    auto index = ( v % 0100 ) * 2;
    v /= 0100;
    *--p = detail::digits_table_base8[index + 1];
    *--p = detail::digits_table_base8[index];
  }
  if( v < 010 ) {
    *--p = '0' + static_cast<char_t>( v );
  } else {
    auto index = v * 2;
    *--p = detail::digits_table_base8[index + 1];
    *--p = detail::digits_table_base8[index];
  }
}

template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
inline void itoa_base8( char_t* const out, unsigned const digits, T const v ) noexcept {
  if( v < 0 ) {
    itoa_base8( out, digits, typename std::make_unsigned<T>::type( -v ) );
  } else {
    itoa_base8( out, digits, typename std::make_unsigned<T>::type( v ) );
  }
}

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline void itoa_base16u( char_t* const out, unsigned const digits, T const x ) noexcept {
  auto v = x;
  char_t* p = out + digits;
  while( v >= 0x100 ) {
    auto index = ( v % 0x100 ) * 2;
    v /= 0x100;
    *--p = detail::digits_table_base16u[index + 1];
    *--p = detail::digits_table_base16u[index];
  }
  if( v < 10 ) {
    *--p = '0' + static_cast<char_t>( v );
  } else if( v < 0x10 ) {
    *--p = 'A' + static_cast<char_t>( v - 10 );
  } else {
    auto index = v * 2;
    *--p = detail::digits_table_base16u[index + 1];
    *--p = detail::digits_table_base16u[index];
  }
}

template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
inline void itoa_base16u( char_t* const out, unsigned const digits, T const v ) noexcept {
  if( v < 0 ) {
    itoa_base16u( out, digits, typename std::make_unsigned<T>::type( -v ) );
  } else {
    itoa_base16u( out, digits, typename std::make_unsigned<T>::type( v ) );
  }
}

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, int> = 0>
inline void itoa_base16l( char_t* const out, unsigned const digits, T const x ) noexcept {
  auto v = x;
  char_t* p = out + digits;
  while( v >= 0x100 ) {
    auto index = ( v % 0x100 ) * 2;
    v /= 0x100;
    *--p = detail::digits_table_base16l[index + 1];
    *--p = detail::digits_table_base16l[index];
  }
  if( v < 10 ) {
    *--p = '0' + static_cast<char_t>( v );
  } else if( v < 0x10 ) {
    *--p = 'a' + static_cast<char_t>( v - 10 );
  } else {
    auto index = v * 2;
    *--p = detail::digits_table_base16l[index + 1];
    *--p = detail::digits_table_base16l[index];
  }
}

template <typename T, std::enable_if_t<std::is_signed<T>::value, int> = 0>
inline void itoa_base16l( char_t* const out, unsigned const digits, T const v ) noexcept {
  if( v < 0 ) {
    itoa_base16l( out, digits, typename std::make_unsigned<T>::type( -v ) );
  } else {
    itoa_base16l( out, digits, typename std::make_unsigned<T>::type( v ) );
  }
}

} // namespace unclassified::format
