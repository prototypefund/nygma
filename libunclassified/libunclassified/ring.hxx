// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/support.hxx>

#include <atomic>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified {

// @see:
//   - <https://github.com/MengRao/WFMPMC>
//   - <https://probablydance.com/2019/12/30/measuring-mutexes-spinlocks-and-how-bad-the-linux-scheduler-really-is/>

template <std::int64_t Count, std::size_t Size>
class ring {
  static_assert( Count >= 2 );
  static_assert( is_power_of_two_v<Count>, "Count must be power of two" );
  static_assert( is_power_of_two_v<Size>, "Size must be power of two" );

  struct block {
    using storage_type = typename std::aligned_storage<Size, CACHE_ALIGN>::type;
    alignas( CACHE_ALIGN ) std::atomic<std::int64_t> _token;
    storage_type _data;
  };

  alignas( CACHE_ALIGN ) std::atomic<std::int64_t> _wr_idx;
  alignas( CACHE_ALIGN ) std::atomic<std::int64_t> _rd_idx;
  block _blocks[Count];

 public:
  inline ring() : _wr_idx{ 0 }, _rd_idx{ 0 } {
    for( auto i = 0; i < Count; i++ ) { _blocks[i]._token.store( i, std::memory_order_relaxed ); }
  }

  ring( ring const& ) = delete;
  ring& operator=( ring const& ) = delete;

  ring( ring&& ) = delete;
  ring& operator=( ring&& ) = delete;

  ~ring() {}

  inline int64_t size() const noexcept {
    return _wr_idx.load( std::memory_order_relaxed ) - _rd_idx.load( std::memory_order_relaxed );
  }

  inline bool empty() const noexcept { return size() <= 0; }

  inline bool full() const noexcept { return size() >= static_cast<std::int64_t>( Count ); }

  inline auto write_idx() noexcept { return _wr_idx.fetch_add( 1, std::memory_order_relaxed ); }

  template <typename T>
  inline T* write_ptr( std::int64_t const idx ) noexcept {
    static_assert( sizeof( T ) <= Size && alignof( T ) <= Size, "T does not fit into storage" );
    static_assert( alignof( T ) <= CACHE_ALIGN, "T has incompatible alignment" );

    auto& blk = _blocks[idx & ( Count - 1 )];
    if( blk._token.load( std::memory_order_acquire ) != idx ) { return nullptr; }
    return reinterpret_cast<T*>( &blk._data );
  }

  inline void write_commit( std::int64_t const idx ) noexcept {
    auto& blk = _blocks[idx & ( Count - 1 )];
    blk._token.store( ~idx, std::memory_order_release );
  }

  template <typename BackoffStrategy, typename F>
  inline auto push( F const f ) noexcept ->
      typename std::enable_if<noexcept( f( &_blocks[0]._data ) ), void>::type {
    auto idx = write_idx();
    auto& blk = _blocks[idx & ( Count - 1 )];
    for( BackoffStrategy bo{}; blk._token.load( std::memory_order_acquire ) != idx; ++bo ) bo();
    f( &blk._data );
    blk._token.store( ~idx, std::memory_order_release );
  }

  inline std::int64_t read_idx() noexcept { return _rd_idx.fetch_add( 1, std::memory_order_relaxed ); }

  template <typename T>
  inline T const* read_ptr( std::int64_t const idx ) const noexcept {
    static_assert( sizeof( T ) <= Size && alignof( T ) <= Size, "T does not fit into storage" );
    static_assert( alignof( T ) <= CACHE_ALIGN, "T has incompatible alignment" );

    auto const& blk = _blocks[idx & ( Count - 1 )];
    if( blk._token.load( std::memory_order_acquire ) != ~idx ) { return nullptr; }
    return reinterpret_cast<T const*>( &blk._data );
  }

  inline void read_commit( std::int64_t const idx ) noexcept {
    auto& blk = _blocks[idx & ( Count - 1 )];
    blk._token.store( idx + Count, std::memory_order_release );
  }

  template <typename BackoffStrategy, typename F>
  inline auto pull( F const f ) noexcept -> typename std::enable_if<
      noexcept( f( static_cast<typename block::storage_type const*>( &_blocks[0]._data ) ) ),
      void>::type {
    auto idx = read_idx();
    auto& blk = _blocks[idx & ( Count - 1 )];
    for( BackoffStrategy bo{}; blk._token.load( std::memory_order_acquire ) != ~idx; ++bo ) bo();
    f( &blk._data );
    blk._token.store( idx + Count, std::memory_order_release );
  }
};

} // namespace unclassified

#ifdef __clang__
#pragma clang diagnostic pop
#endif
