// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/ring.hxx>
#include <libunclassified/support.hxx>

#include <atomic>
#include <filesystem>
#include <iostream>

extern "C" {
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified::nanomsg {

enum class mode { create, attach };

namespace fs = std::filesystem;

namespace detail {

struct alignas( CACHE_ALIGN ) meta {
  std::uint64_t _ring_count;
  std::uint64_t _ring_size;
  std::uint64_t _data_size;

  meta( std::uint64_t const ring_count, std::uint64_t const ring_size, std::uint64_t const data_size )
    : _ring_count{ ring_count }, _ring_size{ ring_size }, _data_size{ data_size } {}
};

static_assert( sizeof( meta ) == CACHE_ALIGN );
} // namespace detail
template <std::size_t Count, std::size_t Size>
class nanomsg {
 public:
  // clang-format off
  using ring_type = ring<Count, Size>;
  static constexpr std::size_t k_page_size = ( 4u << 10 );
  static constexpr std::size_t k_ring_size = sizeof( ring_type );
  static constexpr std::size_t k_data_size =
    align_up<std::size_t>(sizeof( detail::meta ) + k_ring_size, k_page_size );
  static constexpr std::size_t k_page_count = k_data_size / k_page_size;
  static constexpr std::size_t k_wasted_space = k_data_size - k_ring_size;
  // clang-format on

 private:
  fs::path const _path;
  mode const _mode;
  ring_type* _ring;
  void* _data;
  bool _creator;

  static void* create( fs::path const& path ) {
    int fd = shm_open( path.c_str(), O_RDWR | O_CREAT | O_EXCL, 0600 );
    if( fd < 0 ) {
      throw std::system_error( errno, std::system_category(), "nanomsg::create: open() failed" );
    }
    if( auto rc = ftruncate( fd, k_data_size ); rc != 0 ) {
      close( fd );
      throw std::system_error( errno, std::system_category(), "nanomsg::create(): ftruncate() failed" );
    }
    auto p = mmap( nullptr, k_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( p == MAP_FAILED ) {
      close( fd );
      throw std::system_error( errno, std::system_category(), "nanomsg::create: mmap() failed" );
    }

    close( fd );

    new( p ) detail::meta( Count, Size, k_data_size );
    new( reinterpret_cast<std::byte*>( p ) + sizeof( detail::meta ) ) ring_type();

    return p;
  }

  static void* attach( fs::path const& path ) {
    int fd = shm_open( path.c_str(), O_RDWR, 0600 );
    if( fd < 0 ) {
      throw std::system_error( errno, std::system_category(), "nanomsg::attach: open() failed" );
    }
    auto p = mmap( nullptr, k_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( p == MAP_FAILED ) {
      close( fd );
      throw std::system_error( errno, std::system_category(), "nanomsg::attach: mmap() failed" );
    }

    close( fd );

    detail::meta const* m = reinterpret_cast<detail::meta const*>( p );

    if( m->_ring_count != Count || m->_ring_size != Size || m->_data_size != k_data_size ) {
      munmap( p, k_data_size );
      throw std::runtime_error( "nanomsg::attach: invalid ring meta data" );
    }

    return p;
  }

 public:
  inline nanomsg( fs::path const& path, mode const mode ) : _path{ path }, _mode{ mode } {
    switch( _mode ) {
      case mode::create: _data = create( path ); break;
      case mode::attach: _data = attach( path ); break;
    }
    _creator = mode == mode::create;
    _ring = reinterpret_cast<ring_type*>( reinterpret_cast<std::byte*>( _data ) +
                                          sizeof( detail::meta ) );
  }

  ~nanomsg() {
    munmap( _data, k_data_size );
    // only unlink if we created the ring
    if( _mode == mode::create ) unlink( _path.c_str() );
  }

  inline auto& ring() noexcept { return *_ring; }
};

inline void unlink( fs::path const& path ) noexcept { shm_unlink( path.c_str() ); }

} // namespace unclassified::nanomsg

#ifdef __clang__
#pragma clang diagnostic pop
#endif

