// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/bytestring.hxx>
#include <libnygma/mmap.hxx>
#include <libnygma/packet-view.hxx>

#include <algorithm>

namespace emptyspace {

using namespace unsafe;

namespace pcap {

struct format {
  using type = std::uint32_t;
  enum : type {
    // standard libpcap format.
    PCAP_MSEC = 0xa1b2c3d4,

    // alexey Kuznetzov's modified libpcap format.
    PCAP_KUZNETZOV = 0xa1b2cd34,

    // Francisco Mesquita <francisco.mesquita@radiomovel.pt>
    PCAP_FMESQUITA = 0xa1b234cd,

    // Navtel Communcations' format, with nanosecond timestamps
    PCAP_NAVTEL = 0xa12b3c4d,

    // for seconds/nanoseconds timestamps by Ulf Lamping <ulf.lamping@web.de>
    PCAP_NSEC = 0xa1b23c4d,
  };
};

struct linktype {
  using type = std::uint32_t;
  enum : type { en10mb = 0x01 };
};

static constexpr std::size_t PCAP_HEADERSZ = 24;
static constexpr std::size_t PACKET_HEADERSZ = 16;

} // namespace pcap

template <endianess E, pcap::format::type F>
class pcap_view {
 public:
  static constexpr endianess ENDIANESS = E;
  static constexpr pcap::format::type FORMAT = F;

  static_assert( FORMAT == pcap::format::PCAP_MSEC || FORMAT == pcap::format::PCAP_NSEC );

  u32 _raw_magic;
  u16 _raw_version_major;
  u16 _raw_version_minor;
  u32 _raw_thiszone;
  u32 _raw_sigfigs;
  u32 _raw_snaplen;
  u32 _raw_linktype;
  bool _valid;

  std::unique_ptr<bytestring_view> _data;

  pcap_view() = delete;

  inline explicit pcap_view( std::unique_ptr<bytestring_view> bs ) noexcept
    : _data{ std::move( bs ) } {
    _valid = _data->size() >= pcap::PCAP_HEADERSZ;
    if( ! _valid ) { return; }
    _data->template istream<ENDIANESS>() //
        >> _raw_magic //
        >> _raw_version_major //
        >> _raw_version_minor //
        >> _raw_thiszone //
        >> _raw_sigfigs //
        >> _raw_snaplen //
        >> _raw_linktype;
  }

  pcap_view( pcap_view const& ) = delete;
  pcap_view& operator=( pcap_view const& ) = delete;

  pcap_view( pcap_view&& ) = default;
  pcap_view& operator=( pcap_view&& other ) = default;

  template <typename Fn>
  inline void for_each( Fn&& f ) const noexcept {
    std::size_t block_offset = pcap::PCAP_HEADERSZ;
    packet_view packet;
    auto is = _data->template istream_at<ENDIANESS>( block_offset );
    while( is.available() > pcap::PACKET_HEADERSZ ) {
      u32 raw_tv_sec, raw_tv_nsec, raw_caplen, raw_snaplen;
      is >> raw_tv_sec >> raw_tv_nsec >> raw_caplen >> raw_snaplen;
      auto const pkt_size = std::min( raw_caplen, raw_snaplen );
      if( pkt_size > is.available() ) { break; }
      auto const tv_sec = std::uint64_t( raw_tv_sec );
      auto tv_nsec = std::uint64_t( raw_tv_nsec );
      if constexpr( FORMAT == pcap::format::PCAP_MSEC ) { tv_nsec *= 1'000'000ull; }
      packet._stamp = tv_sec * 1'000'000'000ull + tv_nsec;
      packet._slice = is.slice( pkt_size );
      is.advance( pkt_size );
      auto const packet_offset = block_offset + pcap::PACKET_HEADERSZ;
      block_offset += pkt_size + pcap::PACKET_HEADERSZ;
      f( packet, packet_offset );
    }
  }

  class cursor {
    bytestring_istream<ENDIANESS> _data;
    packet_view _packet;
    bool _done;

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = packet_view const&;

    explicit constexpr cursor( bytestring_view const* data, bool const done = false ) noexcept
      : _data{ *data }, _done{ done } {
      // skip pcap file header
      _data.advance( pcap::PCAP_HEADERSZ );
      // parse first packet
      next();
    }

   private:
    inline void next() noexcept {
      if( eop() ) {
        _done = true;
        return;
      }
      u32 raw_tv_sec, raw_tv_nsec, raw_caplen, raw_snaplen;
      _data >> raw_tv_sec >> raw_tv_nsec >> raw_caplen >> raw_snaplen;
      auto const pkt_size = std::min( raw_caplen, raw_snaplen );
      auto const tv_sec = std::uint64_t( raw_tv_sec );
      auto tv_nsec = std::uint64_t( raw_tv_nsec );
      if constexpr( FORMAT == pcap::format::PCAP_MSEC ) { tv_nsec *= 1'000'000ull; }
      _packet._stamp = tv_sec * 1'000'000'000ull + tv_nsec;
      _packet._slice = _data.slice( pkt_size );
      //_packet._valid = pkt_size <= _data.available();
      _data.advance( pkt_size );
    }

   public:
    inline bool eop() const noexcept { return _data.available() < pcap::PACKET_HEADERSZ; }

    inline value_type operator*() const noexcept { return _packet; }

    inline auto& operator++() noexcept {
      next();
      return *this;
    }

    inline auto operator++( int ) noexcept {
      auto c = *this;
      ++*this;
      return c;
    }

    inline bool operator==( cursor const& other ) const noexcept {
      return _done == other._done && _data == other._data;
    }

    inline bool operator!=( cursor const& other ) const noexcept { return _done != other._done; }
  };

  inline bool valid() const noexcept { return _valid; }

  using const_interator_type = cursor;
  const_interator_type begin() const noexcept { return const_interator_type{ _data.get() }; }

  const_interator_type end() const noexcept { return const_interator_type{ _data.get(), true }; }
};

template <endianess E, pcap::format::type F>
class pcap_block_view {
 public:
  static constexpr endianess ENDIANESS = E;
  static constexpr pcap::format::type FORMAT = F;

  static_assert( FORMAT == pcap::format::PCAP_MSEC || FORMAT == pcap::format::PCAP_NSEC );

  u32 _raw_magic;
  u16 _raw_version_major;
  u16 _raw_version_minor;
  u32 _raw_thiszone;
  u32 _raw_sigfigs;
  u32 _raw_snaplen;
  u32 _raw_linktype;
  bool _valid;

  std::unique_ptr<block_view> _data;

  explicit pcap_block_view( std::unique_ptr<block_view> data ) noexcept : _data{ std::move( data ) } {
    auto const bs = _data->prefetch( 0 );
    _valid = bs.size() >= pcap::PCAP_HEADERSZ;
    if( not _valid ) { return; }
    bs.template istream<ENDIANESS>() //
        >> _raw_magic //
        >> _raw_version_major //
        >> _raw_version_minor //
        >> _raw_thiszone //
        >> _raw_sigfigs //
        >> _raw_snaplen //
        >> _raw_linktype;
  }

  pcap_block_view( pcap_block_view const& ) = delete;
  pcap_block_view& operator=( pcap_block_view const& ) = delete;

  pcap_block_view( pcap_block_view&& ) = default;
  pcap_block_view& operator=( pcap_block_view&& ) = default;

  static constexpr std::uint64_t to_ns( std::uint32_t const raw_tv_nsec ) noexcept {
    auto const tv_nsec = static_cast<std::uint64_t>( raw_tv_nsec );
    if constexpr( FORMAT == pcap::format::PCAP_MSEC ) {
      return tv_nsec * 1'000'000ull;
    } else {
      return tv_nsec;
    }
  }

  static constexpr std::uint64_t to_timestamp_ns(
      std::uint32_t const raw_tv_sec, std::uint32_t const raw_tv_nsec ) noexcept {
    auto const tv_sec = static_cast<std::uint64_t>( raw_tv_sec );
    auto const tv_nsec = to_ns( raw_tv_nsec );
    return tv_sec * 1'000'000'000ull + tv_nsec;
  }

  template <typename Fn>
  inline void for_each( Fn&& f ) const noexcept {
    unsigned block_offset = pcap::PCAP_HEADERSZ;
    std::uint64_t total_offset = 0;
    packet_view packet;
    bool done = false;
    while( not done ) {
      auto data = _data->prefetch( total_offset );
      auto is = data.template istream_at<ENDIANESS>( block_offset );
      while( is.available() > pcap::PACKET_HEADERSZ ) {
        u32 raw_tv_sec, raw_tv_nsec, raw_caplen, raw_snaplen;
        is >> raw_tv_sec >> raw_tv_nsec >> raw_caplen >> raw_snaplen;
        auto const pkt_size = std::min( raw_caplen, raw_snaplen );
        if( pkt_size > is.available() ) { break; }
        packet._stamp = to_timestamp_ns( raw_tv_sec, raw_tv_nsec );
        packet._slice = is.slice( pkt_size );
        is.advance( pkt_size );
        auto const packet_offset = total_offset + block_offset + pcap::PACKET_HEADERSZ;
        block_offset += pkt_size + pcap::PACKET_HEADERSZ;
        f( packet, packet_offset );
      }
      total_offset += block_offset;
      block_offset = 0;
      done = _data->end();
    }
  }

  packet_view const slice(
      std::uint64_t const offset, std::size_t const size_estimate = 8196u ) const noexcept {
    if( offset < pcap::PCAP_HEADERSZ + pcap::PACKET_HEADERSZ ) { return {}; }
    auto const packet_offset = offset - pcap::PACKET_HEADERSZ;
    if( not _data->in_cached_range( packet_offset, size_estimate ) ) {
      _data->prefetch( packet_offset );
    }
    auto const slice = _data->slice( packet_offset, size_estimate );
    auto is = slice.template istream<ENDIANESS>();
    std::uint32_t raw_tv_sec, raw_tv_nsec, raw_caplen, raw_snaplen;
    is >> raw_tv_sec >> raw_tv_nsec >> raw_caplen >> raw_snaplen;
    auto const packet_size = std::min( raw_caplen, raw_snaplen );
    if( packet_size > is.available() ) { return {}; } // TODO: retry with correct size estimate
    auto const stamp = to_timestamp_ns( raw_tv_sec, raw_tv_nsec );
    return { stamp, is.cursor(), packet_size };
  }

  class cursor {
    block_view* _block;
    bytestring_view _data{ nullptr, 0 };
    std::size_t _block_offset;
    std::uint64_t _total_offset;
    packet_view _packet;
    bool _done;

   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = packet_view const&;

    explicit constexpr cursor( block_view* block, bool const done = false ) noexcept
      : _block{ block }, _done{ done } {
      _total_offset = 0;
      if( _done ) { return; }
      _data = _block->prefetch( 0 );
      // skip pcap file header
      _block_offset = pcap::PCAP_HEADERSZ;
      // parse first packet
      next();
    }

    inline std::size_t available() const noexcept { return _data.size() - _block_offset; }

   private:
    inline void next() noexcept {
      if( available() < pcap::PACKET_HEADERSZ ) {
      try_next_block:
        if( _block->end() ) {
          _done = true;
          return;
        }
        _total_offset += _block_offset;
        _data = _block->prefetch( _total_offset );
        _block_offset = 0;
        if( _data.size() < pcap::PACKET_HEADERSZ ) {
          _done = true;
          return;
        }
      }
      auto is = _data.template istream_at<ENDIANESS>( _block_offset );
      u32 raw_tv_sec, raw_tv_nsec, raw_caplen, raw_snaplen;
      is >> raw_tv_sec >> raw_tv_nsec >> raw_caplen >> raw_snaplen;
      auto const pkt_size = std::min( raw_caplen, raw_snaplen );
      if( pkt_size > is.available() ) { goto try_next_block; }
      _packet._stamp = to_timestamp_ns( raw_tv_sec, raw_tv_nsec );
      _packet._slice = is.slice( pkt_size );
      _block_offset += pkt_size + is.offset();
    }

   public:
    constexpr value_type operator*() const noexcept { return _packet; }

    inline auto& operator++() noexcept {
      next();
      return *this;
    }

    inline auto operator++( int ) noexcept {
      auto c = *this;
      ++*this;
      return c;
    }

    constexpr bool operator==( cursor const& other ) const noexcept { return _done == other._done; }

    constexpr bool operator!=( cursor const& other ) const noexcept { return _done != other._done; }
  };

  inline bool valid() const noexcept { return _valid; }

  using const_interator_type = cursor;

  const_interator_type begin() const noexcept { return const_interator_type{ _data.get() }; }
  const_interator_type end() const noexcept { return const_interator_type{ nullptr, true }; }
};

namespace pcap {

enum class error_code {
  OK,
  UNSUPPORTED_PCAP_FORMAT,
  UNKNOWN_PCAP_FORMAT,
  INVALID_PCAP_FILESIZE,
  INVALID_VIEW,
};

template <
    typename T,
    typename DataView,
    template <endianess E, format::type F>
    typename PcapView,
    endianess E>
static inline error_code specialize0(
    std::unique_ptr<DataView>&& view, std::uint32_t magic, T&& t ) noexcept {
  switch( magic ) {
    case format::PCAP_NSEC: {
      using pcap_view_type = PcapView<E, format::PCAP_NSEC>;
      pcap_view_type pv{ std::move( view ) };
      t( pv );
      return error_code::OK;
    }
    case format::PCAP_MSEC: {
      using pcap_view_type = PcapView<E, format::PCAP_MSEC>;
      pcap_view_type pv{ std::move( view ) };
      t( pv );
      return error_code::OK;
    }
    case format::PCAP_KUZNETZOV: return error_code::UNSUPPORTED_PCAP_FORMAT;
    case format::PCAP_FMESQUITA: return error_code::UNSUPPORTED_PCAP_FORMAT;
    case format::PCAP_NAVTEL: return error_code::UNSUPPORTED_PCAP_FORMAT;
    default: return error_code::UNKNOWN_PCAP_FORMAT;
  }
}

template <typename T>
static inline error_code with( bytestring_view const bs, T&& t ) noexcept {
  if( bs.size() < PCAP_HEADERSZ ) { return error_code::INVALID_PCAP_FILESIZE; }
  if( bs.rd8() == std::byte( 0xa1 ) ) {
    constexpr endianess BE{ endianess::BE };
    auto const magic = bs.rd32<BE>();
    return specialize0<T, bytestring_view, pcap_view, BE>(
        std::make_unique<bytestring_view>( bs ),
        magic,
        std::forward<T>( t ) );
  } else {
    constexpr endianess LE{ endianess::LE };
    auto const magic = bs.rd32<LE>();
    return specialize0<T, bytestring_view, pcap_view, LE>(
        std::make_unique<bytestring_view>( bs ),
        magic,
        std::forward<T>( t ) );
  }
}

template <typename T>
static inline error_code with( std::unique_ptr<block_view> view, T&& t ) noexcept {
  if( not view->is_ok() ) { return error_code::INVALID_VIEW; }
  auto const bs = view->prefetch( 0 );
  if( bs.size() < PCAP_HEADERSZ ) { return error_code::INVALID_PCAP_FILESIZE; }
  if( bs.rd8() == std::byte( 0xa1 ) ) {
    constexpr endianess BE{ endianess::BE };
    auto const magic = bs.rd32<BE>();
    return specialize0<T, block_view, pcap_block_view, BE>(
        std::move( view ),
        magic,
        std::forward<T>( t ) );
  } else {
    constexpr endianess LE{ endianess::LE };
    auto const magic = bs.rd32<LE>();
    return specialize0<T, block_view, pcap_block_view, LE>(
        std::move( view ),
        magic,
        std::forward<T>( t ) );
  }
}

} // namespace pcap

} // namespace emptyspace
