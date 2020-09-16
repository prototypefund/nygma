// License: RoundRobin-1.0.0

#include <filesystem>
#include <string_view>

namespace t3tch {

struct compile_patterns_config {

  std::filesystem::path _patterns{ "/non-existent" };
  std::string _mode{ "block" };
  std::string _pattern_filter{ "pure" };

  compile_patterns_config() {}
};

void t3_command_compile_patterns( compile_patterns_config const& cfg );

} // namespace t3tch
