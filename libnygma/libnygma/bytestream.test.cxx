// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/bytestream.hxx>

namespace {

constexpr std::byte operator"" _b( unsigned long long const x ) noexcept {
  return static_cast<std::byte>( x );
}

emptyspace::pest::suite basic( "bytestream suite", []( auto& test ) {
  using namespace emptyspace::pest;
  namespace stream = emptyspace::bytestream;

  test( "construct cfile_ostream via path", []( auto& expect ) {
    auto os = stream::cfile_ostream{ "/tmp/xxx" };
    expect( os.valid(), equal_to( true ) );
    expect( os.invalid(), not_equal_to( true ) );
    expect( os.ok(), equal_to( true ) );
  } );

  test( "construct cfile_ostream in-memory", []( auto& expect ) {
    std::byte bytes[2];
    {
      auto os = stream::cfile_ostream{ bytes };
      expect( os.valid(), equal_to( true ) );
      expect( os.invalid(), equal_to( false ) );
      os.write( 0x13_b );
      os.write( 0x37_b );
    }
    expect( bytes[0], equal_to( 0x13_b ) );
    expect( bytes[1], equal_to( 0x37_b ) );
  } );

  test( "in-memory cfile_ostream is overflow safe", []( auto& expect ) {
    std::byte bytes[2];
    {
      auto os = stream::cfile_ostream{ bytes };
      expect( os.valid(), equal_to( true ) );
      expect( os.invalid(), equal_to( false ) );
      os.write( 0x13_b );
      os.write( 0x37_b );
      expect( os.ok(), equal_to( true ) );
      os.write( 0x23_b );
      expect( os.ok(), equal_to( false ) );
    }
    expect( bytes[0], equal_to( 0x13_b ) );
    expect( bytes[1], equal_to( 0x37_b ) );
  } );
} );

} // namespace

int main() {
  basic( std::clog );
  return ( EXIT_SUCCESS );
}
