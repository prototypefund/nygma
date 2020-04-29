// SPDX-License-Identifier: UNLICENSE

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct index_file_dependencies {

  std::vector<std::filesystem::path> _i4;
  std::vector<std::filesystem::path> _i6;
  std::vector<std::filesystem::path> _ix;
  std::vector<std::filesystem::path> _iy;

  void gather( std::filesystem::path const& root, std::filesystem::path const& strem );
};

}
