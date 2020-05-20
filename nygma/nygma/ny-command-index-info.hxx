// SPDX-License-Identifier: UNLICENSE

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct index_info_config {
  // via command line
  std::filesystem::path _path{ "/non-existent" };

  index_info_config() {}
};

void ny_command_index_info( index_info_config const& cfg );

} // namespace nygma
