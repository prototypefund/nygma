// SPDX-License-Identifier: BlueOak-1.0.0

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct reverse_slice_config {
  // via command line
  std::filesystem::path _path{ "/non-existent" };
  std::filesystem::path _root;
  std::filesystem::path _out{ "-" };
  std::string _key_iy;

  reverse_slice_config() {}
};

void ny_command_reverse_slice_by( reverse_slice_config const& cfg );

} // namespace nygma
