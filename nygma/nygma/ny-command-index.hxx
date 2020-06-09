// SPDX-License-Identifier: BlueOak-1.0.0

#include <filesystem>
#include <string_view>

namespace nygma {

enum class compression_method {
  BITPACK,
  STREAMVBYTE,
  NONE,
};

namespace {
inline std::string_view const to_string( compression_method const m ) {
  switch( m ) {
    case compression_method::BITPACK: return "BITPACK";
    case compression_method::STREAMVBYTE: return "STREAMVBYTE";
    case compression_method::NONE: return "NONE";
  }
  return "UNKOWN";
}
} // namespace

struct index_pcap_config {

  std::filesystem::path _path{ "/non-existent" };
  compression_method _method_i4{ compression_method::NONE };
  compression_method _method_i6{ compression_method::NONE };
  compression_method _method_ix{ compression_method::NONE };
  compression_method _method_iy{ compression_method::NONE };

  index_pcap_config() {}
};

void ny_command_index_pcap( index_pcap_config const& cfg );

} // namespace nygma
