// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <cstddef>
#include <cstdio>

#include <filesystem>
#include <type_traits>

namespace nygma {

//--not-endianess-safe-bytestreams--------------------------------------------

struct bytestream_status {
  using type = unsigned;
  enum : type {
    OK = 0,
    FAILED = 1 << 0,
  };
};

template <typename Stream>
class bytestream_ostream {

  // post-mortem status
  bytestream_status::type _status{ bytestream_status::OK };

 public:
  using char_type = std::byte;

 private:
  Stream& downcast() noexcept { return static_cast<Stream&>( *this ); }

  Stream const& downcast() const noexcept { return static_cast<Stream const&>( *this ); }

 private:
  bytestream_ostream() = default;
  friend Stream;

 public:
  auto status() const noexcept { return _status; }

  auto ok() const noexcept { return _status == bytestream_status::OK; }

  bytestream_status::type write( std::byte const* const p, std::size_t const n ) noexcept {
    auto& self = downcast();
    auto const rc = self._write_bytes_( p, n );
    _status |= rc;
    return rc;
  }

  bytestream_status::type write( std::byte const b ) noexcept {
    auto& self = downcast();
    auto const rc = self._write_byte_( b );
    _status = rc;
    return rc;
  }

  template <typename T>
  auto write( T const* const p, std::size_t const n ) noexcept
      -> std::enable_if_t<std::is_trivial_v<T> && not std::is_same_v<T, std::byte>,
                          bytestream_status::type> {
    auto& self = downcast();
    auto const rc = self._write_trivial_( p, n );
    _status |= rc;
    return rc;
  }

  std::int64_t current_position() const noexcept { return downcast()._current_position_(); }

  void sync() noexcept { downcast()._sync_(); }
};

class cfile_ostream : public bytestream_ostream<cfile_ostream> {

  std::FILE* _handle;

 public:
  cfile_ostream( std::filesystem::path const& p ) noexcept { _handle = std::fopen( p.c_str(), "wb" ); }

  cfile_ostream( std::byte* const p, std::size_t n ) noexcept { _handle = ::fmemopen( p, n, "wb" ); }

  template <std::size_t N>
  cfile_ostream( std::byte ( &bytes )[N] ) noexcept {
    _handle = ::fmemopen( bytes, N, "wb" );
  }

  ~cfile_ostream() noexcept {
    if( _handle ) { std::fclose( _handle ); }
  }

  cfile_ostream( cfile_ostream const& ) = delete;
  cfile_ostream& operator=( cfile_ostream const& ) = delete;

  cfile_ostream& operator=( cfile_ostream&& ) = delete;
  cfile_ostream( cfile_ostream&& o ) : _handle{ o._handle } { o._handle = nullptr; }

  bool inline valid() const noexcept { return _handle != nullptr; }
  bool inline invalid() const noexcept { return _handle == nullptr; }

  bytestream_status::type _write_byte_( std::byte const b ) noexcept {
    if( invalid() ) { return bytestream_status::FAILED; }
    if( auto rc = std::fputc( static_cast<int>( b ), _handle ); rc == EOF ) {
      return bytestream_status::FAILED;
    }
    return bytestream_status::OK;
  }

  bytestream_status::type _write_bytes_( std::byte const* const p, std::size_t const n ) noexcept {
    if( invalid() ) { return bytestream_status::FAILED; }
    if( auto rc = std::fwrite( p, n, 1, _handle ); rc != 1 ) { return bytestream_status::FAILED; }
    return bytestream_status::FAILED;
  }

  template <typename T>
  bytestream_status::type _write_trivial_( T const* const p, std::size_t const n ) noexcept {
    if( invalid() ) { return bytestream_status::FAILED; }
    if( auto rc = std::fwrite( p, sizeof( T ), n, _handle ); rc < n ) {
      return bytestream_status::FAILED;
    }
    return bytestream_status::OK;
  }

  std::int64_t _current_position_() const noexcept {
    if( invalid() ) { return -1; }
    return static_cast<std::int64_t>( ::ftello( _handle ) );
  }

  void _sync_() noexcept {
    if( invalid() ) { return; }
    fflush( _handle );
  }
};

} // namespace nygma
