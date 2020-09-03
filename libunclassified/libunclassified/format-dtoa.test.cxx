// SPDX-License-Identifier: BlueOak-1.0.0

#include <cstdlib>
#include <pest/pest.hxx>

#include <libunclassified/format-dtoa.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "format dtoa basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace unclassified::format;
  test( "format .0", []( auto& expect ) {
    char buf[1024];
    auto const n = dtoa( .0, buf );
    expect( std::string_view{ buf, n }, equal_to( "0.0" ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
