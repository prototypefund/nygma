// SPDX-License-Identifier UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/bytestring.hxx>

namespace {

using namespace emptyspace;

std::byte const data1[1] = {
    std::byte( 0x23 ),
};

std::byte const data2[2] = {
    std::byte( 0x42 ),
    std::byte( 0x23 ),
};

std::byte const data4[4] = {
    std::byte( 0x42 ),
    std::byte( 0x23 ),
    std::byte( 0x13 ),
    std::byte( 0x37 ),
};

std::byte const data8[8] = {
    std::byte( 0x42 ),
    std::byte( 0x23 ),
    std::byte( 0x13 ),
    std::byte( 0x37 ),
    std::byte( 0x13 ),
    std::byte( 0x37 ),
    std::byte( 0x23 ),
    std::byte( 0x42 ),
};

emptyspace::pest::suite basic( "bytestring suite", []( auto& test ) {
  using namespace emptyspace::pest;

  constexpr endianess LE = endianess::LE;
  constexpr endianess BE = endianess::BE;

  static_assert( std::is_trivially_destructible_v<bytestring_view> );
  static_assert( std::is_trivially_destructible_v<bytestring_istream<BE>> );
  static_assert( std::is_trivially_destructible_v<bytestring_istream<LE>> );

  test( "bytestring_istream", []( auto& expect ) {
    bytestring_istream<LE> bs{ data1 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.size(), equal_to( 1u ) );
    expect( bs.cursor(), equal_to( bs.data() ) );
    expect( bs.available( 1 ), equal_to( true ) );
    expect( bs.rd8(), equal_to( std::byte( 0x23 ) ) );
    expect( bs.available( 2 ), equal_to( false ) );
    expect( throws<std::out_of_range>( [&]() { bs.available_exn( 2 ); } ) );
    expect( bs.rd8(), equal_to( std::byte( 0x23 ) ) );
    bs.advance( 1 );
    expect( bs.available( 1 ), equal_to( false ) );
  } );

  test( "rd16<LE>", []( auto& expect ) {
    bytestring_istream<BE> const bs{ data2 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 2 ), equal_to( true ) );
    expect( bs.rd16<LE>(), equal_to( 0x2342u ) );
  } );

  test( "rd16<BE>", []( auto& expect ) {
    bytestring_istream<LE> const bs{ data2 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 2 ), equal_to( true ) );
    expect( bs.rd16<BE>(), equal_to( 0x4223u ) );
  } );

  test( "rd32<LE>", []( auto& expect ) {
    bytestring_istream<BE> const bs{ data4 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<LE>(), equal_to( 0x37132342u ) );
  } );

  test( "rd32<BE>", []( auto& expect ) {
    bytestring_istream<LE> const bs{ data4 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<BE>(), equal_to( 0x42231337u ) );
  } );

  test( "rd64<LE>", []( auto& expect ) {
    bytestring_istream<BE> const bs{ data8 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 8 ), equal_to( true ) );
    expect( bs.rd64<LE>(), equal_to( 0x4223371337132342ull ) );
  } );

  test( "rd64<BE>", []( auto& expect ) {
    bytestring_istream<LE> const bs{ data8 };
    expect( bs.offset(), equal_to( 0u ) );
    expect( bs.available( 8 ), equal_to( true ) );
    expect( bs.rd64<BE>(), equal_to( 0x4223133713372342ull ) );
  } );

  test( "rd32<LE>rd32<LE>", []( auto& expect ) {
    bytestring_istream<BE> bs{ data8 };
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<LE>(), equal_to( 0x37132342u ) );
    bs.advance( 4 );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<LE>(), equal_to( 0x42233713u ) );
    bs.advance( 4 );
    expect( ! bs.available( 1 ), equal_to( true ) );
    //expect_THROWS( bs.available_exn( 1 ) );
  } );

  test( "rd32<BE>rd32<BE>", []( auto& expect ) {
    bytestring_istream<LE> bs{ data8 };
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<BE>(), equal_to( 0x42231337u ) );
    bs.advance( 4 );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<BE>(), equal_to( 0x13372342u ) );
    bs.advance( 4 );
    expect( bs.available( 1 ), equal_to( false ) );
    //expect_THROWS( bs.available_exn( 1 ) );
  } );

  test( "rd32<BE>rd32<LE>", []( auto& expect ) {
    bytestring_istream<BE> bs{ data8 };
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<BE>(), equal_to( 0x42231337u ) );
    bs.advance( 4 );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<LE>(), equal_to( 0x42233713u ) );
    bs.advance( 4 );
    expect( bs.available(), equal_to( 0u ) );
    expect( bs.available( 1 ), equal_to( false ) );
    //expect_THROWS( bs.available_exn( 1 ) );
  } );

  test( "rd32<LE>rd32<BE>", []( auto& expect ) {
    bytestring_istream<LE> bs{ data8 };
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<LE>(), equal_to( 0x37132342u ) );
    bs.advance( 4 );
    expect( bs.available( 4 ), equal_to( true ) );
    expect( bs.rd32<BE>(), equal_to( 0x13372342u ) );
    bs.advance( 4 );
    expect( bs.available(), equal_to( 0u ) );
    expect( bs.available( 1 ), equal_to( false ) );
    //expect_THROWS( bs.available_exn( 1 ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
