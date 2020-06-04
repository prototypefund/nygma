// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/bytestring.hxx>

namespace nygma {

using bytestring_view = unclassified::bytestring_view;

struct packet_view {
  std::uint64_t _stamp{ 0 };
  std::uint32_t _hash{ 0 };
  std::uint32_t _port{ 0 };
  bytestring_view _slice;

  constexpr packet_view() noexcept : _slice{ nullptr, 0 } {}

  constexpr packet_view( std::uint32_t const tv_sec, std::uint32_t const tv_nsec,
                         bytestring_view const& slice ) noexcept
    : _stamp{ std::uint64_t( tv_sec ) * 1'000'000'000ull + tv_nsec }, _slice{ slice } {}

  constexpr packet_view( std::uint64_t const stamp, std::byte const* byte,
                         std::size_t const size ) noexcept
    : _stamp{ stamp }, _slice{ byte, size } {}

  constexpr packet_view( std::uint64_t const stamp, std::uint32_t const hash, std::byte const* byte,
                         std::size_t const size ) noexcept
    : _stamp{ stamp }, _hash{ hash }, _slice{ byte, size } {}

  constexpr packet_view( std::uint64_t const stamp, std::uint32_t const hash, std::uint32_t const port,
                         std::byte const* byte, std::size_t const size ) noexcept
    : _stamp{ stamp }, _hash{ hash }, _port{ port }, _slice{ byte, size } {}

  constexpr packet_view( std::uint64_t const stamp, bytestring_view const& slice ) noexcept
    : _stamp{ stamp }, _slice{ slice } {}

  constexpr packet_view( bytestring_view const& slice ) noexcept : _slice{ slice } {}

  packet_view( packet_view const& ) = default;
  packet_view& operator=( packet_view const& ) = default;

  packet_view( packet_view&& ) = default;
  packet_view& operator=( packet_view&& ) = default;

  constexpr auto size() const noexcept { return _slice.size(); }

  constexpr auto data() const noexcept { return _slice.data(); }

  constexpr auto stamp() const noexcept { return _stamp; }
};

} // namespace nygma
