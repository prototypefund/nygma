#include <cassert>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <libunclassified/nanolog.hxx>
#include <libunclassified/version.hxx>

using namespace unclassified;

int main() {

  nanolog::init();

  nanolog::use( std::make_unique<nanolog::stdio>( std::clog ) );

  nlog( lvl::i, "hello world" );

  nanolog::shutdown();
}
