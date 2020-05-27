// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libnygma/bytestream.hxx>
#include <libriot/index-builder.hxx>
#include <libriot/compress-vbyte.hxx>

#include <array>
#include <vector>

namespace riot {

// at most 4 values ( 4bits )
struct tag {
  using type = std::uint8_t;
  enum : type {
    CBLOCK,
    KBLOCK,
    MBLOCK,
    OBLOCK,
  };
};

struct method {
  using type = std::uint8_t;
  enum : type {
    BP128D1,
    BP256D1,
    SVB128D1,
    SVB256D1,
    SVQ4x0D1,
    SVQ3x2D1,
    UC128 = 0xfe,
    UC256 = 0xff,
  };
};

template <typename KeyType>
constexpr auto to_keytype() {
  constexpr auto size = sizeof( KeyType );
  if constexpr( size == 2 ) {
    return 0b00;
  } else if constexpr( size == 4 ) {
    return 0b01;
  } else if constexpr( size == 8 ) {
    return 0b10;
  } else if constexpr( size == 16 ) {
    return 0b11;
  }
}

struct block_subtype {
  using type = std::uint8_t;
  enum : type {
    NONE,
    CBEGIN,
    CCONT,
    RESERVED,
  };
};

union encoding {
  struct {
    std::uint8_t _tag : 2;
    std::uint8_t _type : 2;
    std::uint8_t _clen : 2;
    std::uint8_t _ulen : 2;
  };
  std::byte _value;
  void tag( tag::type const t ) noexcept { _tag = t; }
  void ulen( std::uint8_t const ulen ) noexcept { _ulen = ulen; }
  void clen( std::uint8_t const clen ) noexcept { _clen = clen; }

  static constexpr encoding kblock() noexcept {
    return { ._tag = tag::KBLOCK, block_subtype::NONE, 0, 0 };
  }

  static constexpr encoding oblock() noexcept {
    return { ._tag = tag::OBLOCK, block_subtype::NONE, 0, 0 };
  }

  static constexpr encoding cblock( block_subtype::type const subty ) noexcept {
    return { ._tag = tag::CBLOCK, subty, 0, 0 };
  }
};

static_assert( sizeof( encoding ) == 1 );

template <typename OStream>
struct bytestream_serializer_base {
  static constexpr std::uint32_t MAGIC[2] = { 0x13371337u, 0x41414141u };
  using ostream_type = nygma::bytestream_ostream<OStream>;

  ostream_type& _os;

  bytestream_serializer_base( ostream_type& os ) : _os{ os } {}

 protected:
  template <typename T, std::size_t BlockLen>
  void encode_record(
      encoding enc, T const* p, std::size_t const n, std::size_t const count ) noexcept {
    std::byte meta[16];
    std::byte* meta_p = meta;
    auto const encoded_size = static_cast<std::uint32_t>( n * sizeof( T ) );
    auto const usize = static_cast<std::uint32_t>( count );
    if( usize < BlockLen ) {
      auto [utag, ulen] = vbyte::encode( meta_p + 1, usize );
      auto [ctag, clen] = vbyte::encode( meta_p + 1 + ulen, encoded_size );
      enc.ulen( utag );
      enc.clen( ctag );
      meta[0] = enc._value;
      _os.write( meta_p, 1 + ulen + clen );
      _os.write( p, n );
    } else {
      auto [ctag, clen] = vbyte::encode( meta_p + 1, encoded_size );
      // assuming the blocksize is always encodable as 3byte `vbkey`
      // 0b11 means then that the block was fully populated
      enc.ulen( 0b11 );
      enc.clen( ctag );
      meta[0] = enc._value;
      _os.write( meta, 1 + clen );
      _os.write( p, n );
    }
  }

  template <typename KeyType>
  void encode_meta_record(
      std::uint32_t const kb,
      std::uint32_t const ob,
      std::uint64_t const sb,
      method::type const kmethod,
      method::type const vmethod ) noexcept {
    // total META record size = 32bytes
    _os.write( MAGIC, 1 );
    _os.write( std::byte( kmethod ) );
    _os.write( std::byte( vmethod ) );
    _os.write( std::byte( 0x23 ) );
    _os.write( std::byte( to_keytype<KeyType>() ) );
    _os.write( &kb, 1 );
    _os.write( &ob, 1 );
    _os.write( &sb, 1 );
    _os.write( MAGIC, 2 );
  }

 public:
  std::uint32_t current_position() const noexcept {
    return static_cast<std::uint32_t>( _os.current_position() );
  }
};

template <std::size_t KBlockLen, std::size_t VBlockLen, typename OStream>
struct bytestream_serializer : public bytestream_serializer_base<OStream> {
  static_assert( KBlockLen == 128 || KBlockLen == 256 );
  static_assert( VBlockLen == 128 || VBlockLen == 256 );

  static constexpr method::type KMETHOD = KBlockLen == 128 ? method::UC128 : method::UC256;
  static constexpr method::type VMETHOD = VBlockLen == 128 ? method::UC128 : method::UC256;

  template <typename... Args>
  bytestream_serializer( Args&&... args )
    : bytestream_serializer_base<OStream>( std::forward<Args>( args )... ) {}

  template <typename T, std::size_t BlockLen>
  void encode_cblock( T const* p, std::size_t const n, bool const begin ) noexcept {
    auto const enc = encoding::cblock( begin ? block_subtype::CBEGIN : block_subtype::CCONT );
    this->template encode_record<T, BlockLen>( enc, p, n, n );
  }

  template <typename T, std::size_t BlockLen>
  void encode_oblock( T const* p, std::size_t const n ) noexcept {
    auto const enc = encoding::oblock();
    this->template encode_record<T, BlockLen>( enc, p, n, n );
  }

  template <typename T, std::size_t BlockLen>
  void encode_kblock( T const* p, std::size_t const n ) noexcept {
    auto const enc = encoding::kblock();
    this->template encode_record<T, BlockLen>( enc, p, n, n );
  }

  template <typename KeyType>
  void encode_mblock( offset_type const kb, offset_type const ob, std::uint64_t const sb ) noexcept {
    this->template encode_meta_record<KeyType>( kb, ob, sb, KMETHOD, VMETHOD );
  }
};

template <typename T>
struct uc128_serializer : public bytestream_serializer<128, 128, T> {};

template <typename T>
uc128_serializer( T ) -> uc128_serializer<T>;

template <typename T>
struct uc256_serializer : public bytestream_serializer<256, 256, T> {};

template <typename T>
uc256_serializer( T ) -> uc256_serializer<T>;

template <
    typename OStream,
    method::type KMethod,
    typename KCompressor,
    method::type VMethod,
    typename VCompressor>
struct compressing_serializer : public bytestream_serializer_base<OStream> {

  using kcompressor_type = KCompressor;
  using vcompressor_type = VCompressor;
  static constexpr method::type KMETHOD = KMethod;
  static constexpr method::type VMETHOD = VMethod;

  using offset_type = std::uint32_t;
  using key_type = std::uint32_t;

  std::array<std::byte, vcompressor_type::estimate_compressed_size()> _scrtch_chunks;
  std::array<std::byte, kcompressor_type::estimate_compressed_size()> _scrtch_keys;
  std::array<std::byte, vcompressor_type::estimate_compressed_size()> _scrtch_offsets;

  template <typename... Args>
  compressing_serializer( Args&&... args )
    : bytestream_serializer_base<OStream>( std::forward<Args>( args )... ) {}

  template <typename T, std::size_t BlockLen>
  auto encode_cblock( T const* p, std::size_t const n, bool const begin ) noexcept
      -> std::enable_if_t<BlockLen == vcompressor_type::BLOCKLEN, void> {
    using VC = vcompressor_type;
    auto const enc = encoding::cblock( begin ? block_subtype::CBEGIN : block_subtype::CCONT );
    auto const aligned = align_up<VC::STEPLEN>( n );
    auto* const compressed_p = _scrtch_chunks.data();
    auto compressed_size = vcompressor_type::encode( p, aligned, compressed_p );
    this->template encode_record<std::byte, VC::BLOCKLEN>( enc, compressed_p, compressed_size, n );
  }

  template <typename T, std::size_t BlockLen>
  auto encode_oblock( T const* p, std::size_t const n ) noexcept
      -> std::enable_if_t<BlockLen == vcompressor_type::BLOCKLEN, void> {
    using VC = vcompressor_type;
    auto const enc = encoding::oblock();
    auto const aligned = align_up<VC::STEPLEN>( n );
    auto* const compressed_p = _scrtch_offsets.data();
    auto compressed_size = vcompressor_type::encode( p, aligned, compressed_p );
    this->template encode_record<std::byte, VC::BLOCKLEN>( enc, compressed_p, compressed_size, n );
  }

  template <typename T, std::size_t BlockLen>
  auto encode_kblock( T const* p, std::size_t const n ) noexcept
      -> std::enable_if_t<BlockLen == kcompressor_type::BLOCKLEN, void> {
    using KC = kcompressor_type;
    auto const enc = encoding::kblock();
    auto const aligned = align_up<KC::STEPLEN>( n );
    auto* const compressed_p = _scrtch_keys.data();
    auto compressed_size = KC::encode( p, aligned, compressed_p );
    this->template encode_record<std::byte, KC::BLOCKLEN>( enc, compressed_p, compressed_size, n );
  }

  template <typename KeyType>
  void encode_mblock( offset_type const kb, offset_type const ob, std::uint64_t const sb ) noexcept {
    static_assert( std::is_same_v<key_type, KeyType> );
    this->template encode_meta_record<key_type>( kb, ob, sb, KMETHOD, VMETHOD );
  }
};

} // namespace emptyspace::index
