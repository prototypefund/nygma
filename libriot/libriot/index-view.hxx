// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/mmap.hxx>
#include <libriot/compress-bitpack-simd.hxx>
#include <libriot/compress-streamvbyte-simd.hxx>
#include <libriot/compress-vbyte.hxx>
#include <libriot/index-builder.hxx>
#include <libriot/index-resultset.hxx>
#include <libriot/index-serializer.hxx>
#include <libunclassified/bytestring.hxx>

#include <array>
#include <iterator>
#include <ostream>
#include <vector>

namespace riot {

namespace unsafe = unclassified::unsafe;
using bytestring_view = unclassified::bytestring_view;
using endianess = unclassified::endianess;

namespace detail {

template <std::size_t BlockLen>
struct raw {
  using integer_type = std::uint32_t;
  static constexpr endianess LE = endianess::LE;
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t estimate_compressed_size() noexcept {
    return sizeof( offset_type ) * BlockLen;
  }

  template <typename OutIt>
  static void decode( std::byte const* const in, std::size_t n, std::size_t m, OutIt out ) noexcept {
    auto const* p = in;
    auto const* const end = p + ( ( n >> 2 ) << 2 );
    auto i = m;
    while( p < end && i-- > 0 ) {
      *out++ = unsafe::rd32<LE>( p );
      p += 4;
    }
  }
};

template <std::size_t BlockLen>
struct raw128 {
  using integer_type = __uint128_t;
  static constexpr std::size_t BLOCKLEN = BlockLen;
  static constexpr std::size_t estimate_compressed_size() noexcept {
    return sizeof( offset_type ) * BlockLen;
  }

  template <typename OutIt>
  static void decode( std::byte const* const in, std::size_t n, std::size_t m, OutIt out ) noexcept {
    static_assert( sizeof( integer_type ) == 16 );
    auto const* p = in;
    auto const* const end = p + ( ( n >> 4 ) << 4 );
    auto i = m;
    while( p < end && i-- > 0 ) {
      // TODO: ensure correct endianess
      __uint128_t x;
      std::memcpy( &x, p, 16 );
      *out++ = x;
      p += 16;
    }
  }
};

template <typename Decode>
struct decode_wrapper {
  using integer_type = typename Decode::integer_type;
  static constexpr std::size_t BLOCKLEN = Decode::BLOCKLEN;
  static constexpr std::size_t estimate_compressed_size() noexcept {
    return Decode::estimate_compressed_size();
  }

  template <typename OutIt>
  static void decode( std::byte const* const in, std::size_t n, std::size_t m_, OutIt out ) noexcept {
    auto const m = std::min( m_, BLOCKLEN );
    integer_type tmp_out[BLOCKLEN];
    // the decoder might access 128bit or 256bit beyond `n`. this is ok-ish because the access ends up
    // in the key, offset and meta data section
    Decode::decode( in, n, tmp_out );
    std::copy( tmp_out, tmp_out + m, out );
  }
};

using svb128d1 = decode_wrapper<riot::streamvbyte::svb128d1_i128>;
using svb256d1 = decode_wrapper<riot::streamvbyte::svb256d1_i128>;
using bp128d1 = decode_wrapper<riot::bitpack::bp128d1>;
using bp256d1 = decode_wrapper<riot::bitpack::bp256d1>;

} // namespace detail

constexpr std::size_t METASZ = 32;
constexpr std::size_t META_OBLOCK_OFFSET = 20;
constexpr std::size_t META_KBLOCK_OFFSET = 24;
constexpr std::size_t META_KEYTY_OFFSET = 25;
constexpr std::size_t META_KMETHOD_OFFSET = 28;
constexpr std::size_t META_VMETHOD_OFFSET = 27;
constexpr std::size_t META_SEGMENT_OFFSET = 16;
constexpr std::uint32_t MAGIC = 0x13371337u;
constexpr std::uint32_t MAGIC2 = 0x41414141u;
constexpr endianess LE = endianess::LE;

// for forward lookups these are same always the same ...
using resultset_forward_value_type = std::uint32_t;
using resultset_forward_traits = detail::std_vector_traits<resultset_forward_value_type>;
using resultset_forward_type = resultset<resultset_forward_traits, detail::resultset_kind::FORWARD>;

// ... for reverse lookup not so much ...
using resultset_reverse_value_32 = std::uint32_t;
using resultset_reverse_value_64 = std::uint64_t;
using resultset_reverse_value_128 = __uint128_t;
using resultset_reverse_traits_32 = detail::std_vector_traits<resultset_reverse_value_32>;
using resultset_reverse_traits_64 = detail::std_vector_traits<resultset_reverse_value_64>;
using resultset_reverse_traits_128 = detail::std_vector_traits<resultset_reverse_value_128>;
using resultset_reverse_32 = resultset<resultset_reverse_traits_32, detail::resultset_kind::REVERSE>;
using resultset_reverse_64 = resultset<resultset_reverse_traits_64, detail::resultset_kind::REVERSE>;
using resultset_reverse_128 = resultset<resultset_reverse_traits_128, detail::resultset_kind::REVERSE>;

template <typename KeyType, typename VC>
class index_view {
 public:
  using key_type = KeyType;
  using value_type = resultset_forward_value_type;
  using resultset_reverse_traits = detail::std_vector_traits<key_type>;
  using resultset_reverse_type = resultset<resultset_reverse_traits, detail::resultset_kind::REVERSE>;

 private:
  bytestring_view const _data;
  std::uint64_t const _segment_offset;
  std::vector<key_type> _keys;
  std::vector<value_type> _offsets;

 public:
  index_view(
      bytestring_view const data,
      std::uint64_t const segment_offset,
      std::vector<key_type>&& keys,
      std::vector<offset_type>&& offsets ) noexcept
    : _data{ data },
      _segment_offset{ segment_offset },
      _keys{ std::move( keys ) },
      _offsets{ std::move( offsets ) } {
    auto const truncate = std::min( _keys.size(), _offsets.size() );
    _keys.resize( truncate );
    _offsets.resize( truncate );
  }

  index_view( index_view const& ) = delete;
  index_view& operator=( index_view const& ) = delete;

  index_view( index_view&& ) = default;
  index_view& operator=( index_view&& ) = default;

 public:
  auto key_count() const noexcept { return _keys.size(); }
  auto segment_offset() const noexcept { return _segment_offset; }

  template <typename OutIt>
  bool lookup_forward( key_type const k, OutIt out ) const noexcept {
    auto it = std::lower_bound( _keys.begin(), _keys.end(), k );
    if( it == _keys.end() || k < *it ) { return false; }
    auto const o = static_cast<std::size_t>( it - _keys.begin() );
    assert( o < _offsets.size() );
    auto const* p = _data.begin() + _offsets[o];
    auto const* const end = _data.end();
    if( p + METASZ >= end ) { return false; }
    encoding enc{ ._value = *p++ };
    if( not ( enc._tag == tag::CBLOCK && enc._type == block_subtype::CBEGIN ) ) { return false; }
    std::size_t total = 0;
    do {
      auto const uncompressed_size = enc._ulen == 0b11 ? VC::BLOCKLEN : vbyte::decode( p, enc._ulen );
      auto const n = enc._ulen == 0b11 ? 0u : enc._ulen + 1;
      auto const m = enc._clen + 1u;
      auto const compressed_size = vbyte::decode( p + n, enc._clen );
      if( p + n + m + compressed_size > end ) { return false; }
      VC::decode( p + n + m, compressed_size, uncompressed_size, out );
      p += n + m + compressed_size;
      total += uncompressed_size;
      enc._value = *p++;
    } while( p < end && enc._tag == tag::CBLOCK && enc._type != block_subtype::CBEGIN );
    return true;
  }

  resultset_forward_type lookup_forward( key_type const k ) const noexcept {
    resultset_forward_type::container_type values;
    auto const rc = lookup_forward( k, std::back_inserter( values ) );
    return resultset_forward_type{ _segment_offset, rc, std::move( values ) };
  }

  template <typename OutIt>
  void output_keys( OutIt& out ) const {
    std::copy( _keys.begin(), _keys.end(), out );
  }

  // TODO
  //
  bool prepare_reverse_lookups() const noexcept { return true; }

  // TODO: implement ( maybe lazy constructed ) reverse lookup
  //   - for lazy construction maybe use a helper function
  //     otherwise the lookup could not be `const`
  //
  resultset_reverse_type lookup_reverse( value_type const k ) const noexcept {
    unused( k );
    return resultset_reverse_type{ _segment_offset };
  }
};

//--type-erasure-of-index_view<>----------------------------------------------

class poly_index_view {
  using key32_t = std::uint32_t;
  using key64_t = std::uint64_t;
  using key128_t = __uint128_t;

  using value_type = resultset_forward_type::value_type;
  using container_type = resultset_forward_traits::container_type;

  struct base {
    virtual ~base() = default;

    virtual resultset_forward_type lookup_forward_32( key32_t const k ) noexcept = 0;
    virtual resultset_forward_type lookup_forward_64( key64_t const k ) noexcept = 0;
    virtual resultset_forward_type lookup_forward_128( key128_t const k ) noexcept = 0;

    virtual resultset_reverse_32 lookup_reverse_32( value_type const v ) noexcept = 0;
    virtual resultset_reverse_64 lookup_reverse_64( value_type const v ) noexcept = 0;
    virtual resultset_reverse_128 lookup_reverse_128( value_type const v ) noexcept = 0;

    virtual std::size_t sizeof_domain_value() const noexcept = 0;
    virtual std::size_t size() const noexcept = 0;
    virtual std::uint64_t segment_offset() const noexcept = 0;
    virtual void output_keys( std::ostream& os ) const noexcept = 0;
  };

  template <typename T, typename VC>
  struct view final : base {
    index_view<T, VC> _view;

    view( index_view<T, VC> iv ) noexcept : _view{ std::move( iv ) } {}

    std::size_t size() const noexcept override { return _view.key_count(); }

    std::size_t sizeof_domain_value() const noexcept override {
      return sizeof( typename index_view<T, VC>::key_type );
    }

    std::uint64_t segment_offset() const noexcept override { return _view.segment_offset(); }

    void output_keys( std::ostream& os ) const noexcept override {
      // TODO: we want a transformation function
      //   - render keys as ip addresses
      //   - render keys as signature ids
      //   - ...
      if constexpr( sizeof( T ) < 16 ) {
        std::ostream_iterator<T> out{ os, "\n" };
        _view.output_keys( out );
      } else {
        os << "<index_view::output_keys: 128bit keys unimplemented>";
      }
    }

    //--forward-lookup-wrappers-----------------------------------------------

    resultset_forward_type lookup_forward_32( key32_t const k ) noexcept override {
      if constexpr( std::is_same_v<key32_t, typename index_view<T, VC>::key_type> ) {
        return _view.lookup_forward( k );
      }
      return resultset_forward_type{ 0 };
    }

    resultset_forward_type lookup_forward_64( key64_t const k ) noexcept override {
      if constexpr( std::is_same_v<key64_t, typename index_view<T, VC>::key_type> ) {
        return _view.lookup_forward( k );
      }
      return resultset_forward_type{ 0 };
    }

    resultset_forward_type lookup_forward_128( key128_t const k ) noexcept override {
      if constexpr( std::is_same_v<key128_t, typename index_view<T, VC>::key_type> ) {
        return _view.lookup_forward( k );
      }
      return resultset_forward_type{ 0 };
    }

    //--reverse-lookup-wrappers-----------------------------------------------

    resultset_reverse_32 lookup_reverse_32( value_type const v ) noexcept override {
      if constexpr( std::is_same_v<
                        resultset_reverse_32,
                        typename index_view<T, VC>::resultset_reverse_type> ) {
        return _view.lookup_reverse( v );
      }
      return resultset_reverse_32{ 0 };
    }

    resultset_reverse_64 lookup_reverse_64( value_type const v ) noexcept override {
      if constexpr( std::is_same_v<
                        resultset_reverse_64,
                        typename index_view<T, VC>::resultset_reverse_type> ) {
        return _view.lookup_reverse( v );
      }
      return resultset_reverse_64{ 0 };
    }

    resultset_reverse_128 lookup_reverse_128( value_type const v ) noexcept override {
      if constexpr( std::is_same_v<
                        resultset_reverse_128,
                        typename index_view<T, VC>::resultset_reverse_type> ) {
        return _view.lookup_reverse( v );
      }
      return resultset_reverse_128{ 0 };
    }
  };

  std::unique_ptr<base> _p;

 public:
  template <typename T, typename VC>
  poly_index_view( index_view<T, VC>&& iv )
    : _p{ std::make_unique<view<T, VC>>( std::forward<index_view<T, VC>>( iv ) ) } {}

  poly_index_view( poly_index_view const& ) = delete;
  poly_index_view& operator=( poly_index_view const& ) = delete;

  poly_index_view( poly_index_view&& ) noexcept = default;
  poly_index_view& operator=( poly_index_view&& ) noexcept = default;

  auto size() const noexcept { return _p->size(); }
  auto key_count() const noexcept { return _p->size(); }
  auto sizeof_domain_value() const noexcept { return _p->sizeof_domain_value(); }
  std::uint64_t segment_offset() const noexcept { return _p->segment_offset(); }
  void output_keys( std::ostream& os ) const noexcept { return _p->output_keys( os ); }

  resultset_forward_type lookup_forward_32( key32_t const k ) const noexcept {
    return _p->lookup_forward_32( k );
  }

  resultset_forward_type lookup_forward_64( key64_t const k ) const noexcept {
    return _p->lookup_forward_64( k );
  }

  resultset_forward_type lookup_forward_128( key128_t const k ) const noexcept {
    return _p->lookup_forward_128( k );
  }

  resultset_reverse_32 lookup_reverse_32( value_type const v ) const noexcept {
    return _p->lookup_reverse_32( v );
  }

  resultset_reverse_64 lookup_reverse_64( value_type const v ) const noexcept {
    return _p->lookup_reverse_64( v );
  }

  resultset_reverse_128 lookup_reverse_128( value_type const v ) const noexcept {
    return _p->lookup_reverse_128( v );
  }
};

//--opening-/-constructing-an-index-view----------------------------------------------------

namespace detail {

template <typename Compressor, typename OutIt>
void build_oblock( bytestring_view const data, OutIt out ) {
  auto const sz = data.size();
  auto const* p = data.data();
  auto const* const end = p + sz - METASZ;
  auto const offset_block = unsafe::rd32<LE>( p + sz - META_OBLOCK_OFFSET );
  if( offset_block + METASZ > sz ) { throw std::runtime_error( "INVALID_OFFSETBLOCK" ); }
  p += offset_block;
  while( p < end ) {
    encoding const enc{ ._value = p[0] };
    if( enc._tag != tag::OBLOCK ) { break; }
    p++;
    auto const uncompressed_size = enc._ulen == 0b11 ? Compressor::BLOCKLEN
                                                     : vbyte::decode( p, enc._ulen );
    auto const n = enc._ulen == 0b11 ? 0u : enc._ulen + 1;
    auto const m = enc._clen + 1u;
    auto const compressed_size = vbyte::decode( p + n, enc._clen );
    if( p + n + m + compressed_size > end ) { throw std::runtime_error( "INVALID_OFFSETBLOCK" ); }
    Compressor::decode( p + n + m, compressed_size, uncompressed_size, out );
    p += n + m + compressed_size;
  }
}

template <typename KeyCompressor, typename OutIt>
void build_kblock( bytestring_view const data, OutIt out ) {
  auto const sz = data.size();
  auto const* p = data.data();
  auto const* const end = p + sz - METASZ;
  auto const key_block = unsafe::rd32<LE>( p + sz - META_KBLOCK_OFFSET );
  if( key_block + METASZ > sz ) { throw std::runtime_error( "INVALID_KEYBLOCK" ); }
  p += key_block;
  while( p < end ) {
    encoding const enc{ ._value = p[0] };
    if( enc._tag != tag::KBLOCK ) { break; }
    p++;
    auto const uncompressed_size = enc._ulen == 0b11 ? KeyCompressor::BLOCKLEN
                                                     : vbyte::decode( p, enc._ulen );
    auto const n = enc._ulen == 0b11 ? 0u : enc._ulen + 1;
    auto const m = enc._clen + 1u;
    auto const compressed_size = vbyte::decode( p + n, enc._clen );
    if( p + n + m + compressed_size > end ) { throw std::runtime_error( "INVALID_KEYBLOCK" ); }
    KeyCompressor::decode( p + n + m, compressed_size, uncompressed_size, out );
    p += n + m + compressed_size;
  }
}

template <typename KC, typename VC, typename F>
static auto from( bytestring_view const data, std::uint64_t const segment_offset, F const f ) {
  using key_t = typename KC::integer_type;
  std::vector<key_t> keys;
  std::vector<offset_type> offsets;
  detail::build_kblock<KC>( data, std::back_inserter( keys ) );
  detail::build_oblock<VC>( data, std::back_inserter( offsets ) );
  return f( index_view<key_t, VC>( data, segment_offset, std::move( keys ), std::move( offsets ) ) );
}

#define _vmethod_                                                                                     \
  do {                                                                                                \
    switch( meta.vmethod ) {                                                                          \
      case method::UC128: return from<KC, detail::raw<128>>( data, meta.segment_offset, f );          \
      case method::UC256: return from<KC, detail::raw<256>>( data, meta.segment_offset, f );          \
      case method::SVB128D1: return from<KC, detail::svb128d1>( data, meta.segment_offset, f );       \
      case method::SVB256D1: return from<KC, detail::svb256d1>( data, meta.segment_offset, f );       \
      case method::BP128D1: return from<KC, detail::bp128d1>( data, meta.segment_offset, f );         \
      case method::BP256D1: return from<KC, detail::bp256d1>( data, meta.segment_offset, f );         \
      default: throw std::runtime_error( "UNSUPPORTED_VALUE_COMPRESSION_METHOD" );                    \
    }                                                                                                 \
  } while( false )

template <typename F>
static auto from( bytestring_view const data, F const f ) {
  auto const sz = data.size();
  if( sz < METASZ ) { throw std::runtime_error( "INVALID_FILESIZE" ); }
  struct meta {
    std::uint32_t magic0;
    std::uint8_t kmethod;
    std::uint8_t vmethod;
    std::uint8_t reserved;
    std::uint8_t keyty;
    std::uint32_t kblock_offset;
    std::uint32_t oblock_voffset;
    std::uint64_t segment_offset;
    std::uint32_t magic1;
    std::uint32_t magic2;
  } meta;

  auto is = data.template istream_at<LE>( sz - METASZ );
  is >> meta.magic0 >> meta.kmethod >> meta.vmethod >> meta.reserved >> meta.keyty >>
      meta.kblock_offset >> meta.oblock_voffset >> meta.segment_offset >> meta.magic1 >> meta.magic2;

  if( meta.magic0 != MAGIC ) { throw std::runtime_error( "INVALID_MAGIC" ); }
  if( meta.keyty != 0b01 && meta.keyty != 0b11 ) { throw std::runtime_error( "INVALID_KEYTYPE" ); }

  // clang-format off
  if( meta.keyty == 0b01 ) {
    switch( meta.kmethod ) {
      case method::UC128: { using KC = detail::raw<128>; _vmethod_; }
      case method::UC256: { using KC = detail::raw<256>; _vmethod_; }
      case method::SVB128D1: { using KC = detail::svb128d1; _vmethod_; }
      case method::SVB256D1: { using KC = detail::svb256d1; _vmethod_; }
      case method::BP128D1: { using KC = detail::bp128d1; _vmethod_; }
      case method::BP256D1: { using KC = detail::bp256d1; _vmethod_; }
      default: throw std::runtime_error( "UNSUPPORTED_32BIT_KEY_COMPRESSION_METHOD" );
    }
  } else {
    switch( meta.kmethod ) {
      case method::UC128: { using KC = detail::raw128<128>; _vmethod_; }
      case method::UC256: { using KC = detail::raw128<256>; _vmethod_; }
      default: throw std::runtime_error( "UNSUPPORTED_128BIT_KEY_COMPRESSION_METHOD" );
    }
  }
  // clang-format on
}

#undef _vmethod_

namespace {

inline auto make_poly_index_view( bytestring_view const data ) {
  return detail::from( data, []( auto&& iv ) { return poly_index_view( std::move( iv ) ); } );
}

} // namespace

} // namespace detail

class index_view_handle {

  std::unique_ptr<nygma::mmap_view> _map;
  poly_index_view _index;

 public:
  index_view_handle( std::filesystem::path const& path )
    : _map{ std::make_unique<nygma::mmap_view>( path ) },
      _index{ detail::make_poly_index_view( _map->view() ) } {}

  // the index view wraps `data` non-owning. make sure it outlasts
  // the lifetime of the index view
  //
  index_view_handle( bytestring_view const data ) : _index{ detail::make_poly_index_view( data ) } {}

  index_view_handle( index_view_handle const& ) = delete;
  index_view_handle& operator=( index_view_handle const& ) = delete;

  index_view_handle( index_view_handle&& ) noexcept = default;
  index_view_handle& operator=( index_view_handle&& ) noexcept = default;

  auto const& operator*() const noexcept { return _index; }
  auto const* operator->() const noexcept { return &_index; }
};

namespace {

inline auto make_poly_index_view( std::filesystem::path const& p ) { return index_view_handle{ p }; }

// the index view wraps `data` non-owning. make sure it outlasts
// the lifetime of the index view
//
inline auto make_poly_index_view( bytestring_view const data ) { return index_view_handle{ data }; }

} // namespace

} // namespace riot
