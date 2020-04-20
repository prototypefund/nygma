// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/support.hxx>
#include <libunclassified/bytestring.hxx>

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdexcept>

extern "C" {
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
}

namespace emptyspace {

namespace fs = std::filesystem;
using bytestring_view = unclassified::bytestring_view;

class mmap_view {

  using value_type = std::byte;

  value_type const* _map;
  std::size_t _size;

 public:
  explicit mmap_view( fs::path const& file ) {
    auto const size = fs::file_size( file );
    if( size == 0 ) { throw std::runtime_error( "mmap_view: file has zero size" ); }
    auto fd = open( file.c_str(), O_RDONLY );
    if( fd < 0 ) {
      throw std::system_error( errno, std::system_category(), "mmap_view: unable to open file" );
    }
    auto const* p = mmap( NULL, size, PROT_READ, MAP_PRIVATE, fd, 0 );
    if( p == MAP_FAILED ) {
      auto const err = errno;
      close( fd );
      throw std::system_error(
          err,
          std::system_category(),
          "mmap_view: unable to `mmap` file contents" );
    }
    close( fd );
    _map = reinterpret_cast<value_type const*>( p );
    _size = size;
  }

  ~mmap_view() { munmap( reinterpret_cast<void*>( const_cast<value_type*>( _map ) ), _size ); }

  mmap_view( mmap_view const& ) = delete;
  mmap_view& operator=( mmap_view const& ) = delete;

  mmap_view( mmap_view&& ) = delete;
  mmap_view& operator=( mmap_view&& ) = delete;

  inline bytestring_view view() const noexcept { return bytestring_view{ _map, _size }; }
};

// XXX: move this to platform/platform.hxx
struct mmap_block_hint {
  using type = int;
  enum : type {
    map_shared = MAP_SHARED,
    map_private = MAP_PRIVATE,
    map_anonymous = MAP_ANONYMOUS,
  };
  // MAP_LOCKED is not available on freebsd
  static constexpr type map_private_locked = map_private | map_anonymous;
};

template <std::size_t Size>
class mmap_block {
  static_assert( is_power_of_two_v<Size>, "Size must be power of two" );
  static_assert( Size % ( 4u << 10 ) == 0, "Size must be multiple of 4k" );

 public:
  using value_type = std::byte;

  static constexpr std::size_t SIZE = Size;

  static inline void* try_mmap(
      mmap_block_hint::type const wanted, mmap_block_hint::type const fallback ) {
    void* const p = mmap( NULL, SIZE, PROT_READ | PROT_WRITE, wanted, -1, 0 );
    if( p == MAP_FAILED ) { return mmap( NULL, SIZE, PROT_READ | PROT_WRITE, fallback, -1, 0 ); }
    return p;
  }

 private:
  value_type* _map;

 public:
  inline explicit mmap_block() : _map{ nullptr } {
    void* const p = try_mmap( mmap_block_hint::map_private_locked, mmap_block_hint::map_private );
    if( p == MAP_FAILED ) { throw std::runtime_error( "mmap_block: mmap() failed" ); }
    _map = static_cast<value_type*>( p );
  }

  ~mmap_block() {
    if( _map != nullptr ) { munmap( static_cast<void*>( _map ), SIZE ); }
  }

  mmap_block( mmap_block const& ) = delete;
  mmap_block& operator=( mmap_block const& ) = delete;

  mmap_block( mmap_block&& other ) noexcept : _map{ nullptr } { other.swap( *this ); }
  mmap_block& operator=( mmap_block&& other ) noexcept {
    other.swap( *this );
    return *this;
  }

  void swap( mmap_block& other ) noexcept {
    using std::swap;
    swap( _map, other._map );
  }

  friend void swap( mmap_block& lhs, mmap_block& rhs ) noexcept { lhs.swap( rhs ); }

  inline bytestring_view view() const noexcept { return bytestring_view( _map, SIZE ); }

  inline auto data() noexcept { return _map; }

  inline auto data() const noexcept { return _map; }

  inline auto data( std::size_t const size ) noexcept { return _map + size; }

  inline auto data( std::size_t const size ) const noexcept { return _map + size; }

  inline auto begin() noexcept { return _map; }

  inline std::byte const* begin() const noexcept { return _map; }

  inline auto end() noexcept { return data( SIZE ); }

  inline std::byte const* end() const noexcept { return data( SIZE ); }

  constexpr auto size() { return SIZE; }
};

struct block_flags {
  using type = int;
  enum : int {
    rd = O_RDONLY,
    wr = O_WRONLY,
    ra = O_RDWR,
    direct = O_DIRECT,
  };
  static constexpr type rdd = rd | direct;
};

class block_view {
 public:
  static constexpr std::size_t BLOCKSZ = 2ul << 20;
  static constexpr auto INVALID = std::numeric_limits<std::uint64_t>::max();

 private:
  mmap_block<BLOCKSZ> _block;
  int _fd{ -1 };
  std::size_t _cached_size{ 0 };
  std::uint64_t _cached_offset{ INVALID };
  bool _end{ false };

 public:
  explicit block_view( fs::path const& path, block_flags::type const flags ) noexcept {
    _fd = open( path.c_str(), flags );
  }

  ~block_view() noexcept {
    if( _fd >= 0 ) { close( _fd ); }
  }

  block_view( block_view const& ) = delete;
  block_view& operator=( block_view const& ) = delete;

  block_view( block_view&& other ) noexcept
    : _block{ std::move( other._block ) },
      _fd{ other._fd },
      _cached_size{ other._cached_size },
      _cached_offset{ other._cached_offset },
      _end{ other._end } {
    other._fd = -1;
    other._cached_size = 0;
    other._cached_offset = INVALID;
    other._end = true;
  }

  block_view& operator=( block_view&& other ) noexcept {
    other.swap( *this );
    return *this;
  }

  void swap( block_view& other ) noexcept {
    using std::swap;
    swap( _block, other._block );
    swap( _fd, other._fd );
    swap( _cached_size, other._cached_size );
    swap( _cached_offset, other._cached_offset );
    swap( _end, other._end );
  }

  friend void swap( block_view& lhs, block_view& rhs ) noexcept { lhs.swap( rhs ); }

  inline auto cached_size() const noexcept { return _cached_size; }

  inline auto cached_offset() const noexcept { return _cached_offset; }

  constexpr auto block_size() const noexcept { return BLOCKSZ; }

  inline auto is_ok() const noexcept { return _fd >= 0; }

  // returns `true` if a previous `read()` yield less than `BLOCKSZ` or failed
  inline auto end() const noexcept { return _end; }

  bool in_cached_range( std::uint64_t const offset, std::size_t size ) const noexcept {
    return offset >= _cached_offset && ( offset + size ) < _cached_offset + _cached_size;
  }

  inline bytestring_view const slice(
      std::uint64_t const offset, std::size_t const size ) const noexcept {
    if( in_cached_range( offset, size ) ) {
      auto const* p = _block.data();
      return bytestring_view{ p + offset - _cached_offset, size };
    }
    return bytestring_view{ nullptr, 0u };
  }

  bytestring_view const prefetch( std::uint64_t const offset ) noexcept {
    if( not is_ok() ) { return bytestring_view{ nullptr, 0u }; }
    // if the cached offset matches just return
    // a bytestring_view with the cached size
    auto* p = _block.data();
    if( offset == _cached_offset ) { return bytestring_view{ p, _cached_size }; }
    std::size_t n = _block.size();
    off_t o = static_cast<off_t>( offset );
    while( n > 0 ) {
      auto rc = pread( _fd, p, n, o );
      if( rc < 0 ) {
        if( errno == EAGAIN || errno == EINTR ) { continue; }
        _end = true;
        _cached_size = 0;
        _cached_offset = INVALID;
      } else if( rc == 0 ) {
        break;
      }
      n -= static_cast<std::size_t>( rc );
      o += rc;
      p += rc;
    }
    _cached_offset = offset;
    _cached_size = _block.size() - n;
    _end = _cached_size < _block.size();
    return bytestring_view{ _block.data(), _block.size() - n };
  }
};

} // namespace emptyspace
