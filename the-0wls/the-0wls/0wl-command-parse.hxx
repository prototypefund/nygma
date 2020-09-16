// License: RoundRobin-1.0.0

#pragma once

#include <filesystem>

namespace owl {

struct parse_config {
  std::filesystem::path _path{ "/non-existent" };
};

void owl_command_parse( parse_config const& config );

} // namespace owl
