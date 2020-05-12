// SPDX-License-Identifier: UNLICENSE

#include <filesystem>
#include <string_view>
#include <vector>

namespace nygma {

struct query_config {
  // via command line
  std::filesystem::path _path{ "/non-existent" };
  std::filesystem::path _root;
  std::filesystem::path _out{ "-" };
  std::string _query;

  query_config() {}
};

void ny_command_query( query_config const& cfg );

} // namespace nygma
