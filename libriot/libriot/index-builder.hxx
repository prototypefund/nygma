// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <array>
#include <cassert>
#include <vector>

namespace riot {

using offset_type = std::uint32_t;

template <class From, class To>
concept convertible_to =
    std::is_convertible_v<From, To>&& requires( std::add_rvalue_reference_t<From> ( &f )() ) {
  static_cast<To>( f() );
};

template <typename T, typename KeyType, std::size_t KBlockLen, std::size_t VBlockLen>
concept Serializer = requires( T s, bool begin, offset_type const* p, std::size_t const n,
                               offset_type const key_begin, offset_type const offset_begin,
                               std::uint64_t const segment_begin ) {
  // clang-format off
  { s.template encode_cblock<offset_type, VBlockLen>( p, n, begin ) } noexcept;
  { s.template encode_kblock<offset_type, KBlockLen>( p, n ) } noexcept;
  { s.template encode_oblock<offset_type, VBlockLen>( p, n ) } noexcept;
  { s.template encode_mblock<KeyType>( key_begin, offset_begin, segment_begin ) } noexcept;
  { s.current_position() } noexcept -> convertible_to<offset_type>;
  // clang-format on
};

template <std::size_t N>
struct is_power_of_two {
  static constexpr bool value = ( N > 1 ) && ! ( N & ( N - 1 ) );
};

template <std::size_t N>
constexpr bool is_power_of_two_v = is_power_of_two<N>::value;

template <std::size_t N>
inline std::size_t align_up( std::size_t const x ) noexcept {
  static_assert( is_power_of_two_v<N> );
  return ( x + ( N - 1 ) ) & ~( N - 1 );
}

template <typename I, std::size_t N>
inline void fill_block( I* p, std::size_t const n ) noexcept {
  std::fill_n( p + n, N - n, p[n - 1] );
}

template <typename T, std::size_t BlockLen, typename Alloc = std::allocator<std::array<T, BlockLen>>>
class chunked_vector {
  static_assert( is_power_of_two_v<BlockLen> );

 public:
  using chunk_type = std::array<T, BlockLen>;
  using vector_type = std::vector<chunk_type, Alloc>;
  using iterator = typename vector_type::iterator;
  using const_iterator = typename vector_type::const_iterator;

 private:
  vector_type _chunks;
  std::size_t _cursor{ 0 };
  T _cached{ 0 }; // we assume a value of `0` is invalid

 public:
  chunked_vector() : _chunks{} {}

  inline void push( T const t ) noexcept {
    if( t == _cached ) { return; }
    _cached = t;
    auto const idx = _cursor / BlockLen;
    auto const offset = _cursor & ( BlockLen - 1 );
    if( offset == 0 ) { _chunks.emplace_back(); }
    _chunks[idx][offset] = t;
    _cursor++;
  }

  iterator begin() noexcept { return _chunks.begin(); }
  const_iterator begin() const noexcept { return _chunks.begin(); }
  const_iterator cbegin() const noexcept { return _chunks.cbegin(); }

  iterator end() noexcept { return _chunks.end(); }
  const_iterator end() const noexcept { return _chunks.end(); }
  const_iterator cend() const noexcept { return _chunks.cend(); }

  std::size_t size() const noexcept { return _cursor; }

  std::size_t chunk_count() const noexcept { return _chunks.size(); }

  auto const& chunk( std::size_t const i ) const noexcept { return _chunks[i]; }

  std::size_t chunk_offset() const noexcept { return _cursor & ( BlockLen - 1 ); }

  T at( std::size_t const i ) const {
    auto const idx = i / BlockLen;
    auto const offset = i & ( BlockLen - 1 );
    return _chunks.at( idx ).at( offset );
  }
};

template <typename Key, template <typename K, typename V> typename Map, std::size_t BlockLen,
          std::size_t VBlockLen = BlockLen,
          typename Alloc = std::allocator<std::array<offset_type, BlockLen>>>
class index_builder {
  static constexpr std::size_t KBLOCKLEN = BlockLen;
  static constexpr std::size_t VBLOCKLEN = VBlockLen;
  using key_type = Key;
  using chunk_index_type = std::uint32_t;
  using map_type = Map<key_type, chunk_index_type>;
  using chunked_vector_type = chunked_vector<offset_type, VBLOCKLEN, Alloc>;
  map_type _index;
  std::vector<chunked_vector_type> _chunks;
  chunk_index_type _last_used_chunk_index{ 0 };

 public:
  index_builder() : _index{} {}

 private:
  void update_chunk( chunk_index_type const i, offset_type const o ) noexcept { _chunks[i].push( o ); }

 public:
  void add( key_type const k, offset_type const o ) noexcept {
    auto it = _index.find( k );
    if( it != _index.end() ) {
      update_chunk( it->second, o );
    } else {
      _index.insert( { k, _last_used_chunk_index } );
      _chunks.emplace_back();
      update_chunk( _last_used_chunk_index, o );
      _last_used_chunk_index++;
    }
  }

  auto key_count() const noexcept { return _index.size(); }

  std::pair<std::size_t, std::size_t> minmax_offset_count() const noexcept {
    if( _index.size() == 0 ) { return { 0, 0 }; }
    auto const [min, max] = std::minmax_element( _index.begin(), _index.end(), [&]( auto& a, auto& b ) {
      return _chunks[a.second].size() < _chunks[b.second].size();
    } );
    return { _chunks[min->second].size(), _chunks[max->second].size() };
  }

  // this invalidates the index builder
  void accept( Serializer<key_type, KBLOCKLEN, VBLOCKLEN> auto& serializer,
               std::uint64_t const segment_begin ) noexcept {

    // - serialize all chunked vectors ( and patch index to external offsets )
    for( auto it = _index.begin(); it != _index.end(); ++it ) {
      // replace `chunk_index_type` with the position
      // reported by the serializer
      auto const chunk_index = it->second;
      auto const o = serializer.current_position();
      if constexpr( requires { it.data(); } ) {
        it.data() = o;
      } else {
        it->second = o;
      }
      auto cs = _chunks[chunk_index];
      auto remaining = cs.size();
      auto begin = true;
      for( auto cit = cs.begin(); cit != cs.end(); ++cit ) {
        auto const used = std::min( remaining, cit->size() );
        if( used != VBLOCKLEN ) { fill_block<offset_type, VBLOCKLEN>( cit->data(), used ); }
        serializer.template encode_cblock<offset_type, VBLOCKLEN>( cit->data(), used, begin );
        remaining -= used;
        begin = false;
      }
      assert( remaining == 0 );
    }

    // - serialize all keys
    auto const keys_begin = serializer.current_position();
    auto keys = 0u;
    key_type keyblock[KBLOCKLEN];
    for( auto it = _index.begin(); it != _index.end(); ++it ) {
      if( keys == KBLOCKLEN ) {
        serializer.template encode_kblock<key_type, KBLOCKLEN>( keyblock, keys );
        keys = 0;
      }
      keyblock[keys] = it->first;
      keys++;
    }
    if( keys > 0 ) {
      fill_block<key_type, KBLOCKLEN>( keyblock, keys );
      serializer.template encode_kblock<key_type, KBLOCKLEN>( keyblock, keys );
    }
    //auto const keys_end = serializer.current_position();

    // - serialize all external offsets
    auto const offsets_begin = serializer.current_position();
    auto offsets = 0u;
    offset_type offsetblock[VBLOCKLEN];
    for( auto it = _index.begin(); it != _index.end(); ++it ) {
      if( offsets == VBLOCKLEN ) {
        serializer.template encode_oblock<offset_type, VBLOCKLEN>( offsetblock, offsets );
        offsets = 0;
      }
      offsetblock[offsets] = it->second;
      offsets++;
    }
    if( offsets > 0 ) {
      fill_block<offset_type, VBLOCKLEN>( offsetblock, offsets );
      serializer.template encode_oblock<offset_type, VBLOCKLEN>( offsetblock, offsets );
    }
    //auto const offsets_end = serializer.current_position();

    // - serialize meta data
    serializer.template encode_mblock<key_type>( keys_begin, offsets_begin, segment_begin );
  }
};

} // namespace riot
