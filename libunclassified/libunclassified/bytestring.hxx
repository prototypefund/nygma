// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace unclassified {

enum class endianess { LE, BE };

namespace unsafe {

using u8 = std::byte;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

template <endianess E>
constexpr std::uint16_t rd16( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u16( p[0] ) | u16( u16( p[1] ) << 8 );
  } else if constexpr( E == endianess::BE ) {
    return u16( u16( p[0] ) << 8 ) | u16( p[1] );
  }
}

template <endianess E>
constexpr std::uint32_t rd24( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u32( p[0] ) | ( u32( p[1] ) << 8 ) | ( u32( p[2] ) << 16 );
  } else if constexpr( E == endianess::BE ) {
    return ( u32( p[0] ) << 16 ) | ( u32( p[1] ) << 8 ) | u32( p[2] );
  }
}

template <endianess E>
constexpr std::uint32_t rd32( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u32( p[0] ) | ( u32( p[1] ) << 8 ) | ( u32( p[2] ) << 16 ) | ( u32( p[3] ) << 24 );
  } else if constexpr( E == endianess::BE ) {
    return ( u32( p[0] ) << 24 ) | ( u32( p[1] ) << 16 ) | ( u32( p[2] ) << 8 ) | u32( p[3] );
  }
}

template <endianess E>
constexpr std::uint64_t rd40( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u64( p[0] ) | ( u64( p[1] ) << 8 ) | ( u64( p[2] ) << 16 ) | ( u64( p[3] ) << 24 ) |
        ( u64( p[4] ) << 32 );
  } else if constexpr( E == endianess::BE ) {
    return ( u64( p[0] ) << 32 ) | ( u64( p[1] ) << 24 ) | ( u64( p[2] ) << 16 ) |
        ( u64( p[3] ) << 8 ) | u64( p[4] );
  }
}

template <endianess E>
constexpr std::uint64_t rd48( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u64( p[0] ) | ( u64( p[1] ) << 8 ) | ( u64( p[2] ) << 16 ) | ( u64( p[3] ) << 24 ) |
        ( u64( p[4] ) << 32 ) | ( u64( p[5] ) << 40 );
  } else if constexpr( E == endianess::BE ) {
    return ( u64( p[0] ) << 40 ) | ( u64( p[1] ) << 32 ) | ( u64( p[2] ) << 24 ) |
        ( u64( p[3] ) << 16 ) | ( u64( p[4] ) << 8 ) | u64( p[5] );
  }
}

template <endianess E>
constexpr std::uint64_t rd56( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u64( p[0] ) | ( u64( p[1] ) << 8 ) | ( u64( p[2] ) << 16 ) | ( u64( p[3] ) << 24 ) |
        ( u64( p[4] ) << 32 ) | ( u64( p[5] ) << 40 ) | ( u64( p[6] ) << 48 );
  } else if constexpr( E == endianess::BE ) {
    return ( u64( p[0] ) << 48 ) | ( u64( p[1] ) << 40 ) | ( u64( p[2] ) << 32 ) |
        ( u64( p[3] ) << 24 ) | ( u64( p[4] ) << 16 ) | ( u64( p[5] ) << 8 ) | u64( p[6] );
  }
}

template <endianess E>
constexpr std::uint64_t rd64( std::byte const* p ) noexcept {
  if constexpr( E == endianess::LE ) {
    return u64( p[0] ) | ( u64( p[1] ) << 8 ) | ( u64( p[2] ) << 16 ) | ( u64( p[3] ) << 24 ) |
        ( u64( p[4] ) << 32 ) | ( u64( p[5] ) << 40 ) | ( u64( p[6] ) << 48 ) | ( u64( p[7] ) << 56 );
  } else if constexpr( E == endianess::BE ) {
    return ( u64( p[0] ) << 56 ) | ( u64( p[1] ) << 48 ) | ( u64( p[2] ) << 40 ) |
        ( u64( p[3] ) << 32 ) | ( u64( p[4] ) << 24 ) | ( u64( p[5] ) << 16 ) | ( u64( p[6] ) << 8 ) |
        u64( p[7] );
  }
}

template <endianess E>
constexpr void wr16( std::byte* const p, std::uint16_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 8 );
    p[1] = b( x );
  }
}

template <endianess E>
constexpr void wr24( std::byte* const p, std::uint32_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 16 );
    p[1] = b( x >> 8 );
    p[2] = b( x );
  }
}

template <endianess E>
constexpr void wr32( std::byte* const p, std::uint32_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 24 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 24 );
    p[1] = b( x >> 16 );
    p[2] = b( x >> 8 );
    p[3] = b( x );
  }
}

template <endianess E>
constexpr void wr40( std::byte* const p, std::uint64_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 24 );
    p[4] = b( x >> 32 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 32 );
    p[1] = b( x >> 24 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 8 );
    p[4] = b( x );
  }
}

template <endianess E>
constexpr void wr48( std::byte* const p, std::uint64_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 24 );
    p[4] = b( x >> 32 );
    p[5] = b( x >> 40 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 40 );
    p[1] = b( x >> 32 );
    p[2] = b( x >> 24 );
    p[3] = b( x >> 16 );
    p[4] = b( x >> 8 );
    p[5] = b( x );
  }
}

template <endianess E>
constexpr void wr56( std::byte* const p, std::uint64_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 24 );
    p[4] = b( x >> 32 );
    p[5] = b( x >> 40 );
    p[6] = b( x >> 48 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 48 );
    p[1] = b( x >> 40 );
    p[2] = b( x >> 32 );
    p[3] = b( x >> 24 );
    p[4] = b( x >> 16 );
    p[5] = b( x >> 8 );
    p[6] = b( x );
  }
}

template <endianess E>
constexpr void wr64( std::byte* const p, std::uint64_t const x ) noexcept {
  using b = std::byte;
  if constexpr( E == endianess::LE ) {
    p[0] = b( x );
    p[1] = b( x >> 8 );
    p[2] = b( x >> 16 );
    p[3] = b( x >> 24 );
    p[4] = b( x >> 32 );
    p[5] = b( x >> 40 );
    p[6] = b( x >> 48 );
    p[7] = b( x >> 56 );
  } else if constexpr( E == endianess::BE ) {
    p[0] = b( x >> 56 );
    p[1] = b( x >> 48 );
    p[2] = b( x >> 40 );
    p[3] = b( x >> 32 );
    p[4] = b( x >> 24 );
    p[5] = b( x >> 16 );
    p[6] = b( x >> 8 );
    p[7] = b( x );
  }
}

} // namespace unsafe

template <endianess>
class bytestring_istream;

class bytestring_view {
  using value_type = std::byte;

 private:
  std::byte const* _data;
  std::size_t _size;

  bytestring_view() = delete;

 public:
  explicit constexpr bytestring_view( std::byte const* data, std::size_t const size ) noexcept
    : _data{ data }, _size{ size } {}

  template <size_t N>
  explicit constexpr bytestring_view( std::byte const ( &data )[N] ) noexcept
    : _data{ data }, _size{ N } {}

  template <size_t N>
  explicit constexpr bytestring_view( char const ( &data )[N] ) noexcept
    : _data{ reinterpret_cast<std::byte const*>( data ) }, _size{ N } {}

  template <size_t N>
  explicit constexpr bytestring_view( unsigned char const ( &data )[N] ) noexcept
    : _data{ reinterpret_cast<std::byte const*>( data ) }, _size{ N } {}

  bytestring_view( bytestring_view const& ) = default;
  bytestring_view& operator=( bytestring_view const& ) = default;

  bytestring_view( bytestring_view&& ) = default;
  bytestring_view& operator=( bytestring_view&& other ) = default;

 private:
  constexpr auto operator[]( std::size_t const idx ) const noexcept { return _data[idx]; }

 public:
  constexpr std::size_t size() const noexcept { return _size; }

  constexpr auto data() const noexcept { return _data; }

  constexpr auto begin() const noexcept { return _data; }

  constexpr auto end() const noexcept { return _data + _size; }

  constexpr auto data( std::size_t const offset ) const noexcept { return _data + offset; }

  constexpr bool available( std::size_t n ) const noexcept { return n <= size(); }

  constexpr void available_exn( std::size_t const n ) const {
    if( n > size() ) { throw std::out_of_range( "bytestring_view::available_exn: n > size()" ); }
  }

  constexpr bytestring_view slice_exn( std::size_t size, std::size_t offset = 0u ) const {
    available_exn( size + offset );
    return bytestring_view{ _data + offset, size };
  }

  constexpr bytestring_view slice( std::size_t size, std::size_t offset = 0 ) const noexcept {
    return bytestring_view{ _data + offset, size };
  }

  inline std::string_view sv() const noexcept {
    static_assert( sizeof( std::byte ) == sizeof( char ) );
    return { reinterpret_cast<char const*>( _data ), _size };
  }

  constexpr std::byte at( std::size_t const idx ) const {
    if( idx < _size ) { return _data[idx]; }
    throw std::out_of_range( "bytestring_view::at: idx >= _sz" );
  }

  constexpr auto rd8( std::size_t const idx = 0 ) const noexcept { return _data[idx]; }

  template <endianess E>
  constexpr auto rd16( std::size_t const idx = 0 ) const noexcept {
    return unsafe::rd16<E>( data( idx ) );
  }

  template <endianess E>
  constexpr auto rd32( std::size_t const idx = 0 ) const noexcept {
    return unsafe::rd32<E>( data( idx ) );
  }

  template <endianess E>
  constexpr auto rd64( std::size_t const idx = 0 ) const noexcept {
    return unsafe::rd64<E>( data( idx ) );
  }

  template <endianess E>
  constexpr auto istream( std::size_t const size, std::size_t const offset = 0 ) const noexcept {
    return bytestring_istream<E>{ data( offset ), size };
  }

  template <endianess E>
  constexpr auto istream() const noexcept {
    return bytestring_istream<E>{ data(), size() };
  }

  template <endianess E>
  constexpr auto istream_at( std::size_t const offset ) const noexcept {
    return bytestring_istream<E>{ data( offset ), size() - offset };
  }
};

template <endianess DefaultEndianess>
class bytestring_istream {

  static constexpr endianess k_endianess = DefaultEndianess;

  using value_type = std::byte;

 private:
  std::byte const* _data;
  std::size_t const _size;
  std::size_t _offset;

  bytestring_istream() = delete;

 public:
  explicit constexpr bytestring_istream( std::byte const* data, std::size_t const size ) noexcept
    : _data{ data }, _size{ size }, _offset{ 0 } {}

  explicit constexpr bytestring_istream( std::byte const* data, std::size_t const size,
                                         std::size_t const offset )
    : _data{ data }, _size{ size }, _offset{ offset } {
    if( _offset > _size ) { throw std::out_of_range( "bytestring_istream: offset > size" ); }
  }

  explicit constexpr bytestring_istream( bytestring_view const& view, std::size_t const offset = 0 )
    : bytestring_istream{ view.data( offset ), view.size() - offset } {}

  template <size_t N>
  explicit constexpr bytestring_istream( std::byte const ( &data )[N] ) noexcept
    : _data{ data }, _size{ N }, _offset{ 0 } {}

  // XXX: maybe skip the trailing `\0' of string literals
  template <size_t N>
  explicit constexpr bytestring_istream( char const ( &data )[N] ) noexcept
    : _data{ reinterpret_cast<std::byte const*>( data ) }, _size{ N }, _offset{ 0 } {}

  template <size_t N>
  explicit constexpr bytestring_istream( unsigned char const ( &data )[N] ) noexcept
    : _data{ reinterpret_cast<std::byte const*>( data ) }, _size{ N }, _offset{ 0 } {}

  bytestring_istream( bytestring_istream const& ) = default;
  bytestring_istream( bytestring_istream&& ) = default;

  bytestring_istream& operator=( bytestring_istream const& ) = default;
  bytestring_istream& operator=( bytestring_istream&& ) = default;

  constexpr std::size_t size() const noexcept { return _size; }

  constexpr auto data() const noexcept { return _data; }

  constexpr auto cursor() const noexcept { return _data + _offset; }

  constexpr auto available() const noexcept { return _size - _offset; }

  constexpr auto offset() const noexcept { return _offset; }

  constexpr bool available( std::size_t n ) const noexcept { return n <= available(); }

  constexpr void available_exn( std::size_t const n ) const {
    if( n > available() ) {
      throw std::out_of_range( "bytestring_istream::available_exn: n > available()" );
    }
  }

  constexpr void advance_exn( std::size_t const n ) {
    available_exn( n );
    _offset += n;
  }

  constexpr void advance( std::size_t const n ) noexcept { _offset += n; }

  constexpr auto slice_exn( std::size_t size ) const {
    available_exn( size );
    return bytestring_view{ cursor(), size };
  }

  constexpr auto slice( std::size_t size ) const noexcept { return bytestring_view{ cursor(), size }; }

  constexpr std::byte at( std::size_t const idx ) const {
    if( idx < _size ) { return _data[idx]; }
    throw std::out_of_range( "bytestring_istream::at: idx >= _size" );
  }

  constexpr bool operator==( bytestring_istream<k_endianess> const& other ) const noexcept {
    return _data == other._data && _size == other._size && _offset == other._offset;
  }

  constexpr bool operator!=( bytestring_istream<k_endianess> const& other ) const noexcept {
    return _data != other._data || _size != other._size || _offset != other._offset;
  }

  // unchecked and unsafe operations -----------------------------------------

  // unchecked access: ensure `available() >= 1`
  constexpr auto rd8() const noexcept { return cursor()[0]; }

  // unchecked access: ensure `available() >= 2`
  template <endianess E>
  constexpr auto rd16() const noexcept {
    return unsafe::rd16<E>( cursor() );
  }

  // unchecked access: ensure `available() >= 4`
  template <endianess E>
  constexpr auto rd32() const noexcept {
    return unsafe::rd32<E>( cursor() );
  }

  // unchecked access: ensure `available() >= 8`
  template <endianess E>
  constexpr auto rd64() const noexcept {
    return unsafe::rd64<E>( cursor() );
  }

  // unchecked access: ensure `available() >= sizeof( T )`
  template <typename T,
            std::enable_if_t<std::is_integral_v<T> &&
                                 ( sizeof( T ) == 2 || sizeof( T ) == 4 || sizeof( T ) == 8 ),
                             int> = 0>
  constexpr auto& operator>>( T& v ) noexcept {
    if constexpr( sizeof( T ) == 2 ) {
      v = unsafe::rd16<k_endianess>( cursor() );
    } else if constexpr( sizeof( T ) == 4 ) {
      v = unsafe::rd32<k_endianess>( cursor() );
    } else if constexpr( sizeof( T ) == 8 ) {
      v = unsafe::rd64<k_endianess>( cursor() );
    }
    advance( sizeof( T ) );
    return *this;
  }

  // unchecked access: ensure `available() >= 1`
  constexpr auto& operator>>( std::uint8_t& v ) noexcept {
    v = static_cast<std::uint8_t>( *cursor() );
    advance( 1 );
    return *this;
  }

  // unchecked access: ensure `available() >= 1`
  constexpr auto& operator>>( unsafe::u8& v ) noexcept {
    v = cursor()[0];
    advance( 1 );
    return *this;
  }
};

} // namespace unclassified
