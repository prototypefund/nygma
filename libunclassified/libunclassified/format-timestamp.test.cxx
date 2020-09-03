// SPDX-License-Identifier: BlueOak-1.0.0

// test cases from:
//   - <https://github.com/niXman/dtf>

#include <pest/pest.hxx>

#include <libunclassified/format-timestamp.hxx>

#include <cstring>
#include <iostream>

namespace {

namespace ts = unclassified::format;
using flags = ts::timestamp_flags;

struct testvals {
  flags::type flags;
  std::size_t exlen;
  const char* exstr;
};

testvals fmt( char ( &buf )[ts::TIMESTAMP_BUFSZ], std::uint64_t const ts, flags::type f ) {
  auto n = ts::format_ts( buf, ts, f );
  buf[n] = 0;
  return { f, n, buf };
}

static testvals const vals[] = {
    { flags::yyyy_mm_dd | flags::sep1 | flags::secs, 19, "2019-01-08 16:50:23" },
    { flags::yyyy_mm_dd | flags::sep1 | flags::msecs, 23, "2019-01-08 16:50:23.006" },
    { flags::yyyy_mm_dd | flags::sep1 | flags::usecs, 26, "2019-01-08 16:50:23.006057" },
    { flags::yyyy_mm_dd | flags::sep1 | flags::nsecs, 29, "2019-01-08 16:50:23.006057057" },
    { flags::yyyy_mm_dd | flags::sep2 | flags::secs, 19, "2019.01.08-16.50.23" },
    { flags::yyyy_mm_dd | flags::sep2 | flags::msecs, 23, "2019.01.08-16.50.23.006" },
    { flags::yyyy_mm_dd | flags::sep2 | flags::usecs, 26, "2019.01.08-16.50.23.006057" },
    { flags::yyyy_mm_dd | flags::sep2 | flags::nsecs, 29, "2019.01.08-16.50.23.006057057" },
    { flags::yyyy_mm_dd | flags::sep3 | flags::secs, 19, "2019.01.08-16:50:23" },
    { flags::yyyy_mm_dd | flags::sep3 | flags::msecs, 23, "2019.01.08-16:50:23.006" },
    { flags::yyyy_mm_dd | flags::sep3 | flags::usecs, 26, "2019.01.08-16:50:23.006057" },
    { flags::yyyy_mm_dd | flags::sep3 | flags::nsecs, 29, "2019.01.08-16:50:23.006057057" },
    { flags::dd_mm_yyyy | flags::sep1 | flags::secs, 19, "2019-01-08 16:50:23" },
    { flags::dd_mm_yyyy | flags::sep1 | flags::msecs, 23, "2019-01-08 16:50:23.006" },
    { flags::dd_mm_yyyy | flags::sep1 | flags::usecs, 26, "2019-01-08 16:50:23.006057" },
    { flags::dd_mm_yyyy | flags::sep1 | flags::nsecs, 29, "2019-01-08 16:50:23.006057057" },
    { flags::dd_mm_yyyy | flags::sep2 | flags::secs, 19, "2019.01.08-16.50.23" },
    { flags::dd_mm_yyyy | flags::sep2 | flags::msecs, 23, "2019.01.08-16.50.23.006" },
    { flags::dd_mm_yyyy | flags::sep2 | flags::usecs, 26, "2019.01.08-16.50.23.006057" },
    { flags::dd_mm_yyyy | flags::sep2 | flags::nsecs, 29, "2019.01.08-16.50.23.006057057" },
    { flags::dd_mm_yyyy | flags::sep3 | flags::secs, 19, "2019.01.08-16:50:23" },
    { flags::dd_mm_yyyy | flags::sep3 | flags::msecs, 23, "2019.01.08-16:50:23.006" },
    { flags::dd_mm_yyyy | flags::sep3 | flags::usecs, 26, "2019.01.08-16:50:23.006057" },
    { flags::dd_mm_yyyy | flags::sep3 | flags::nsecs, 29, "2019.01.08-16:50:23.006057057" } };

emptyspace::pest::suite basic( "format timestamp basic suite", []( auto& test ) {
  using namespace emptyspace::pest;

  test( "test predefined stamps and flags combinations", []( auto& expect ) {
    for( const auto& it : vals ) {
      // 2019-01-08 16:50:23.006057557
      const auto ts = 1546966223006057057ull;
      char buf[ts::TIMESTAMP_BUFSZ];
      auto const r = fmt( buf, ts, it.flags );
      expect( r.exlen, equal_to( it.exlen ) );
      expect( std::strcmp( r.exstr, it.exstr ), equal_to( 0 ) );
    }
  } );

  test( "format and manually zero terminate", []( auto& expect ) {
    // 2019-01-08 16:50:23.000000000
    auto const ts = 1546966223000000000ull;
    char buf[ts::TIMESTAMP_BUFSZ];
    auto const n = ts::format_ts( buf, ts, flags::yyyy_mm_dd | flags::sep1 | flags::msecs );
    buf[n] = 0;
    expect( strcmp( buf, "2019-01-08 16:50:23.000" ), equal_to( 0 ) );
    expect( ( std::string_view{ buf, n } ), equal_to( "2019-01-08 16:50:23.000" ) );
  } );

  test( "format to non zero terminated string", []( auto& expect ) {
    // 2019-01-08 16:50:23.000000000
    auto const ts = 1546966223000000000ull;
    char buf[ts::TIMESTAMP_BUFSZ];
    std::memset( buf, 'F', ts::TIMESTAMP_BUFSZ );
    auto const n = ts::format_ts( buf, ts, flags::yyyy_mm_dd | flags::sep1 | flags::msecs );
    buf[n] = 0;
    expect( strcmp( buf, "2019-01-08 16:50:23.000" ), equal_to( 0 ) );
    expect( ( std::string_view{ buf, n } ), equal_to( "2019-01-08 16:50:23.000" ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
