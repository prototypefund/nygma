// SPDX-License-Identifier: BlueOak-1.0.0

// Copyright (C) 2020-present mb. All rights reserved.
// Copyright (c) 2019 niXman. All rights reserved.
// Copyright (c) 2014 Christian Hansen. All rights reserved.

#pragma once

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

namespace unclassified::format {

using char_t = char;

struct timestamp_flags {
  using type = unsigned;
  enum : type {
    yyyy_mm_dd = 1u << 0u,
    dd_mm_yyyy = 1u << 1u,
    // 2018-12-11 13:58:56
    sep1 = 1u << 2u,
    // 2018.12.11-13.58.59
    sep2 = 1u << 3u,
    // 2018.12.11-13:58:59
    sep3 = 1u << 4u,
    // seconds resolution
    secs = 1u << 5u,
    // milliseconds resolution
    msecs = 1u << 6u,
    // microseconds resolution
    usecs = 1u << 7u,
    // nanoseconds resolution
    nsecs = 1u << 8u
  };
};

namespace {
constexpr std::size_t TIMESTAMP_BUFSZ = 32u;
}

// always in nanoseconds resolution
inline std::uint64_t now() noexcept {
  std::int64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::system_clock::now().time_since_epoch() )
                        .count();
  return static_cast<std::uint64_t>( ts );
}

// timestamp algorithm from github.com/chansen/c-timestamp
// rata die algorithm by peter baum
namespace detail {

inline void rdn_to_ymd( std::uint32_t const rdn, std::uint16_t& yy, std::uint16_t& mm,
                        std::uint16_t& dd ) noexcept {
  // XXX: thread_local?
  static constexpr std::uint16_t _day_offset[13] = {
      0u, 306u, 337u, 0u, 31u, 61u, 92u, 122u, 153u, 184u, 214u, 245u, 275u,
  };

  std::uint32_t Z, H, A, B;
  std::uint16_t y, m, d;

  Z = rdn + 306;
  H = 100 * Z - 25;
  A = H / 3652425;
  B = A - ( A >> 2 );
  y = static_cast<std::uint16_t>( ( 100u * B + H ) / 36525u );
  d = static_cast<std::uint16_t>( B + Z - ( 1461u * y >> 2u ) );
  m = ( 535 * d + 48950 ) >> 14;
  if( m > 12 ) {
    y++;
    m -= 12;
  }

  yy = y;
  mm = m;
  dd = d - _day_offset[m];
}

inline unsigned format_ts_impl( char_t* const ptr, std::uint64_t const ts,
                                unsigned const f = timestamp_flags::yyyy_mm_dd |
                                    timestamp_flags::sep3 | timestamp_flags::msecs ) noexcept {
  static constexpr std::uint32_t _pow10[10] = { 1,      10,      100,      1000,      10000,
                                                100000, 1000000, 10000000, 100000000, 1000000000 };
  /* 1970-01-01T00:00:00 */
  static constexpr std::uint64_t _epoch = UINT64_C( 62135683200 );

  auto const datesep = ( f & timestamp_flags::sep1 ) ? '-' : '.';
  auto const timesep = ( f & timestamp_flags::sep1 ) || ( f & timestamp_flags::sep3 ) ? ':' : '.';
  auto const sepsep = ( f & timestamp_flags::sep1 ) ? ' ' : '-';
  auto const ss = ts / 1'000'000'000ull;
  auto const ps = ts % 1'000'000'000ull;
  auto const precision = ( f & timestamp_flags::secs )
      ? 0u
      : ( f & timestamp_flags::msecs ) ? 3u : ( f & timestamp_flags::usecs ) ? 6u : 9u;

  std::uint64_t sec = ss + _epoch;
  std::uint32_t rdn = static_cast<std::uint32_t>( sec / 86400ul );
  std::uint16_t y, m, d;
  rdn_to_ymd( rdn, y, m, d );

  // clang-format off

  //           1
  // 0123456789012345678
  // yyyy.mm.dd-hh:mm:ss
  char_t* p = ptr;
  std::uint32_t v = sec % 86400;
  p[18] = '0' + ( v % 10 ); v /= 10;
  p[17] = '0' + ( v % 6 ); v /= 6;
  p[16] = timesep;
  p[15] = '0' + ( v % 10 ); v /= 10;
  p[14] = '0' + ( v % 6 ); v /= 6;
  p[13] = timesep;
  p[12] = '0' + ( v % 10 ); v /= 10;
  p[11] = '0' + ( v % 10 );
  p[10] = sepsep;
  p[9] = '0' + ( d % 10 ); d /= 10;
  p[8] = '0' + ( d % 10 );
  p[7] = datesep;
  p[6] = '0' + ( m % 10 ); m /= 10;
  p[5] = '0' + ( m % 10 );
  p[4] = datesep;
  p[3] = '0' + ( y % 10 ); y /= 10;
  p[2] = '0' + ( y % 10 ); y /= 10;
  p[1] = '0' + ( y % 10 ); y /= 10;
  p[0] = '0' + ( y % 10 );
  p += 19;

  if( precision > 0 ) {
    v = static_cast<std::uint32_t>( ps / _pow10[9 - precision] );
    switch( precision ) {
      case 9: p[9] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 8: p[8] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 7: p[7] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 6: p[6] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 5: p[5] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 4: p[4] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 3: p[3] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 2: p[2] = '0' + ( v % 10 ); v /= 10; [[fallthrough]];
      case 1: p[1] = '0' + ( v % 10 );
    }
    p[0] = '.';
    p += 1 + precision;
  }

  // clang-format on

  return static_cast<unsigned>( p - ptr );
}
} // namespace detail

template <std::size_t Size>
inline unsigned format_ts( char_t ( &ptr )[Size], std::uint64_t const ts,
                           unsigned const f = timestamp_flags::yyyy_mm_dd | timestamp_flags::sep3 |
                               timestamp_flags::msecs ) noexcept {
  static_assert( Size >= TIMESTAMP_BUFSZ, "Size must be greater or equal to TIMESTAMP_BUFSZ" );
  return detail::format_ts_impl( ptr, ts, f );
}

} // namespace unclassified::format
