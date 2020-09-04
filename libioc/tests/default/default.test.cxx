// License: RoundRobin-1.0.0

#include <libioc/pattern-parser.hxx>
#include <libioc/version.hxx>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {

constexpr auto SNORT_PURE_PATTERNS = "libioc/tests/data/snort-pure-patterns.ioc";
constexpr auto SNORT_REGEXP_PATTERNS = "libioc/tests/data/snort-regexp-patterns.ioc";

} // namespace

int main() {
  try {
    std::cout << "parsing path = " << SNORT_PURE_PATTERNS << std::endl;
    auto input = std::ifstream{ SNORT_PURE_PATTERNS };
    auto const patterns = ioc::parse( input );
    std::cout << "got " << patterns._patterns.size() << " patterns" << std::endl;
  } catch( std::runtime_error const& e ) {
    std::clog << "error: " << e.what() << std::endl;
  } catch( ... ) { std::clog << "error: unexpected exception" << std::endl; }
}
