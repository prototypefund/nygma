// SPDX-License-Identifier: UNLICENSE

#include <pest/pest.hxx>

#include <libnygma/mmap.hxx>

namespace {

emptyspace::pest::suite basic( "mmap suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace emptyspace;
  test( "can instantiate mmap_block<4096>", []( auto& expect ) {
    mmap_block<4096> blk;
    expect( blk.data() != nullptr, equal_to( true ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
