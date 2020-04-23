// SPDX-License-Identifier: UNLICENSE

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct offset_by_config {
  // via command line
  std::filesystem::path _path{ "/non-existent" };
  std::filesystem::path _root;
  std::string _key_i4;
  std::string _key_i6;
  std::string _key_ix;
  std::string _key_iy;

  offset_by_config() {}
};

void ny_command_offset_by( offset_by_config const& cfg );

} // namespace nygma
