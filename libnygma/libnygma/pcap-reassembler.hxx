// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libnygma/packet-view.hxx>
#include <libnygma/pcap-view.hxx>
#include <libunclassified/bytestring.hxx>

#include <filesystem>

extern "C" {
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
}

namespace nygma {

using bytestring_view = unclassified::bytestring_view;

struct pcap_ostream {
  using handle_type = int;
  using iovec_type = ::iovec;

  bool _owning;
  handle_type _fd;

  pcap_ostream( std::filesystem::path const& path ) noexcept : _owning{ true } {
    _fd = ::open( path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600 );
  }

  pcap_ostream( handle_type const fd ) noexcept : _owning{ false }, _fd{ fd } {}

  ~pcap_ostream() {
    if( _owning && _fd >= 0 ) { ::close( _fd ); }
  }

  pcap_ostream( pcap_ostream const& ) = delete;
  pcap_ostream& operator=( pcap_ostream const& ) = delete;

  pcap_ostream( pcap_ostream&& other ) noexcept : _owning{ false }, _fd{ -1 } { other.swap( *this ); }

  pcap_ostream& operator=( pcap_ostream&& other ) noexcept {
    other.swap( *this );
    return *this;
  }

  void swap( pcap_ostream& other ) noexcept {
    using std::swap;
    swap( _owning, other._owning );
    swap( _fd, other._fd );
  }

  friend void swap( pcap_ostream& lhs, pcap_ostream& rhs ) noexcept { lhs.swap( rhs ); }

  bool ok() const noexcept { return _fd >= 0; }

  bool writev( iovec_type const* const data, std::size_t const n ) const noexcept {
    while( true ) {
      if( auto rc = ::writev( _fd, data, static_cast<int>( n ) ); rc > 0 ) { return true; }
      if( not ( errno == EINTR || errno == EAGAIN ) ) { return false; }
    }
  }

  bool write( std::byte const* const data, std::size_t const n ) const noexcept {
    iovec_type const iov = { .iov_base = const_cast<std::byte*>( data ), .iov_len = n };
    return writev( &iov, 1 );
  }
};

namespace detail {
namespace {

constexpr std::uint32_t pcap_header[6]{
    pcap::format::PCAP_NSEC, 0x00040002, 0, 0, 0xffff, pcap::linktype::en10mb };

}
} // namespace detail

namespace pcap {
namespace {

template <typename View, typename Stream>
inline bool reassemble_begin( [[maybe_unused]] View const& pcap, Stream& os ) noexcept {
  return os.write(
      reinterpret_cast<std::byte const*>( detail::pcap_header ),
      sizeof( detail::pcap_header ) );
}

template <typename View, typename Iter, typename Stream>
inline bool reassemble_stream(
    View& pcap, std::uint64_t const segment_offset, Iter begin, Iter const end, Stream& os ) noexcept {
  using iovec_type = typename Stream::iovec_type;
  std::uint32_t packet_header[4];
  iovec_type iov[2];
  iov[0].iov_base = packet_header;
  iov[0].iov_len = sizeof( packet_header );
  while( begin != end ) {
    auto const p = pcap.slice( *begin + segment_offset );
    auto const stamp = p.stamp();
    auto const& slice = p._slice;
    if( slice.size() == 0u ) { return false; }
    packet_header[0] = static_cast<std::uint32_t>( stamp / 1'000'000'000ull );
    packet_header[1] = static_cast<std::uint32_t>( stamp % 1'000'000'000ull );
    packet_header[2] = static_cast<unsigned>( slice.size() );
    packet_header[3] = static_cast<unsigned>( slice.size() );
    iov[1].iov_base = const_cast<std::byte*>( slice.data() );
    iov[1].iov_len = static_cast<unsigned>( slice.size() );
    if( auto const rc = os.writev( iov, 2 ); not rc ) { return false; }
    begin++;
  }
  return true;
}

template <typename View, typename Iter, typename Stream>
inline bool reassemble_from( View& pcap, Iter begin, Iter const end, Stream& os ) noexcept {
  if( auto const rc = reassemble_begin( pcap, os ); not rc ) { return false; }
  return reassemble_stream( pcap, 0u, begin, end, os );
}

} // namespace
} // namespace pcap

} // namespace nygma
