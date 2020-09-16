// License: RoundRobin-1.0.0

#pragma once

#include <filesystem>

namespace owl {

struct ui_config {
  std::string _workspace_directory;
};

void owl_command_ui( ui_config const& config );

} // namespace owl
