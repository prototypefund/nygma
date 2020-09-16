// License: RoundRobin-1.0.0

#include <filesystem>
#include <string_view>

namespace t3tch {

struct index_pcap_config {

  std::filesystem::path _path{ "/non-existent" };
  std::filesystem::path _patterns{ "/non-existent" };
  std::string _mode{ "regexp" };

  index_pcap_config() {}
};

void t3_command_index_pcap( index_pcap_config const& cfg );

} // namespace t3tch
