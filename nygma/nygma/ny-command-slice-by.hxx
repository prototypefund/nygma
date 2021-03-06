// SPDX-License-Identifier: BlueOak-1.0.0

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct slice_config {
  // via command line
  std::filesystem::path _path{ "/non-existent" };
  std::filesystem::path _root;
  std::filesystem::path _out{ "-" };
  std::string _key_i4;
  std::string _key_i6;
  std::string _key_ix;
  std::string _key_iy;

  slice_config() {}
};

void ny_command_slice_by( slice_config const& cfg );

} // namespace nygma
