// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libunclassified/backoff-strategy.hxx>
#include <libunclassified/format.hxx>
#include <libunclassified/ring.hxx>
#include <libunclassified/support.hxx>

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <variant>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified::nanolog {

namespace fs = std::filesystem;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"
#endif

struct lvl {
  using type = int;
  enum : type {
    m = 0,
    c = 1,
    e = 2,
    w = 3,
    i = 4,
    v = 5,
    y = 6,
    d = 7,
  };
  static constexpr char to_char( type const t ) noexcept {
    constexpr char c[8] = {
        [lvl::m] = 'm', [lvl::c] = 'c', [lvl::e] = 'e', [lvl::w] = 'w',
        [lvl::i] = 'i', [lvl::v] = 'v', [lvl::y] = 'y', [lvl::d] = 'd',
    };
    return c[t & 7];
  }
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

class logger {
 public:
  virtual ~logger() = default;

  virtual void log( char const* data, std::size_t const data_n ) noexcept = 0;
  virtual void flush() noexcept = 0;
};

class stdio final : public logger {
  std::ostream& _out{ std::clog };

 public:
  stdio() = default;
  explicit stdio( std::ostream& out ) : _out{ out } {}

  ~stdio() = default;

  void log( char const* data, std::size_t const data_n ) noexcept final {
    _out.write( data, static_cast<std::streamsize>( data_n ) );
  }

  void flush() noexcept final { _out.flush(); }
};

class fileio final : public logger {
  std::ofstream _out;

 public:
  explicit fileio( fs::path const& path ) : _out{ path, std::ofstream::out | std::ofstream::trunc } {}

  ~fileio() override {
    _out.flush();
    _out.close();
  }

  void log( char const* data, std::size_t const data_n ) noexcept final {
    _out.write( data, static_cast<std::streamsize>( data_n ) );
  }

  void flush() noexcept final { _out.flush(); }
};

namespace detail {

static constexpr size_t CHUNK_BUFSZ = 1024;

struct chunk {
  using value_type = char;

  size_t _offset{ 0 };
  bool _overflow{ false };
  std::array<value_type, CHUNK_BUFSZ> _buffer;

  chunk() = default;

  chunk( chunk const& ) = delete;
  chunk& operator=( chunk const& ) = delete;

  chunk( chunk&& ) = delete;
  chunk& operator=( chunk&& ) = delete;

  template <size_t N>
  friend inline bool operator==( chunk const& lhs, char const ( &rhs )[N] ) {
    if( lhs._overflow ) { return false; }
    if( N != lhs._offset + 1 ) { return false; }
    return std::memcmp( lhs._buffer.data(), rhs, lhs._offset ) == 0;
  }

  inline void push_back( char c ) noexcept {
    if( ! _overflow && _offset < CHUNK_BUFSZ ) {
      _buffer[_offset] = c;
      _offset++;
      return;
    }
    _overflow = true;
  }

  inline void reset() {
    _offset = 0;
    _overflow = false;
  }

  inline auto overflow() { return _overflow; }

  inline std::string_view data() { return std::string_view{ _buffer.data(), _offset }; }
};

enum class message_type { use, log, shutdown };

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-designator"
#endif

struct alignas( CACHE_ALIGN ) message_ {
  message_type _type;
  std::thread::id _thread_id;
  std::uint64_t _stamp;

  union payload {
    struct use {
      logger* _logger;
    } _use;
    struct log {
      lvl::type _lvl;
      chunk* _chunk;
    } _log;
  } _p;

  static inline message_ use( logger* l ) noexcept {
    return { ._type = message_type::use,
             ._thread_id = std::this_thread::get_id(),
             ._stamp = unclassified::format::now(),
             ._p._use._logger = l };
  }

  static inline message_ shutdown() noexcept {
    return {
        ._type = message_type::shutdown,
        ._thread_id = std::this_thread::get_id(),
        ._stamp = unclassified::format::now(),
    };
  }

  static inline message_ log( lvl::type const lvl, chunk* chunk ) noexcept {
    return { ._type = message_type::log,
             ._thread_id = std::this_thread::get_id(),
             ._stamp = unclassified::format::now(),
             ._p._log = { ._lvl = lvl, ._chunk = chunk } };
  }
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

static_assert( sizeof( message_ ) <= CACHE_ALIGN );

class actor {
  static constexpr size_t CHUNK_POOL_ENTRIES = 1024;
  static constexpr size_t MESSAGE_QUEUE_ENTRIES = 64;
  static constexpr std::size_t MESSAGESZ = 256;

  static_assert( sizeof( message_ ) <= MESSAGESZ, "message size must fit into power of two storage" );

  template <std::size_t M, std::size_t N>
  using ring_type = ring<M, N>;
  using backoff_strategy = backoff_strategy::backoff2;
  using chunk_pool_type = ring_type<CHUNK_POOL_ENTRIES, sizeof( chunk* )>;
  using message_queue_type = ring_type<MESSAGE_QUEUE_ENTRIES, MESSAGESZ>;

  chunk_pool_type _chunk_pool;
  message_queue_type _message_queue;
  std::unique_ptr<std::thread> _consumer;
  lvl::type _lvl;

  actor( actor const& ) = delete;
  actor& operator=( actor const& ) = delete;

  actor( actor&& ) = delete;
  actor& operator=( actor&& ) = delete;

 private:
  actor() : _chunk_pool{}, _message_queue{}, _lvl{ lvl::i } {
    for( size_t i = 0; i < CHUNK_POOL_ENTRIES; i++ ) {
      auto const idx = _chunk_pool.write_idx();
      chunk** p{ nullptr };
      while( ( p = _chunk_pool.write_ptr<chunk*>( idx ) ) == nullptr ) std::this_thread::yield();
      *p = new chunk{};
      _chunk_pool.write_commit( idx );
    }
  }

  ~actor() {
    if( _consumer ) { _consumer->join(); }
    _consumer.reset();
    for( size_t i = 0; i < CHUNK_POOL_ENTRIES; i++ ) {
      auto const idx = _chunk_pool.read_idx();
      chunk* const* p{ nullptr };
      int tries = 0;
      while( ( p = _chunk_pool.read_ptr<chunk*>( idx ) ) == nullptr ) {
        if( tries > 1024 * 1024 ) {
          std::cerr << "~nanolog: *** unable to purge chunk queue ***" << std::endl;
          // XXX: queue now inconsistent
          _chunk_pool.read_commit( idx );
          break;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
        tries++;
      }
      delete *p;
      _chunk_pool.read_commit( idx );
    }
  }

 public:
  static auto& instance() noexcept {
    static actor _this{};
    return _this;
  }

  class consumer {
    static constexpr auto TIMESTAMP_FORMAT = format::timestamp_flags::yyyy_mm_dd |
        format::timestamp_flags::msecs | format::timestamp_flags::sep3;

    std::unique_ptr<logger> _log{ std::make_unique<stdio>( std::cerr ) };

   public:
    ~consumer() {
      if( _log ) { _log->flush(); }
    }

   private:
    inline void close() noexcept {
      if( _log ) { _log->flush(); }
    }

    inline void log( actor& ctx, message_ const& msg ) noexcept {
      auto chunk = msg._p._log._chunk;
      auto data = chunk->data();
      _log->log( data.data(), data.size() );
      ctx.release( chunk );
    }

    inline void use( message_ const& msg ) noexcept { _log.reset( msg._p._use._logger ); }

   public:
    inline void operator()() noexcept {
      auto& ctx = actor::instance();
      bool done = false;
      while( ! done ) {
        message_ msg;
        ctx.recv( msg );
        switch( msg._type ) {
          case message_type::log: log( ctx, msg ); break;
          case message_type::use: use( msg ); break;
          case message_type::shutdown:
            done = true;
            close();
            break;
            //default:
        }
      }
    }
  };

  inline void spawn() noexcept {
    if( ! _consumer ) {
      _consumer = std::make_unique<std::thread>( [] {
        consumer nc;
        nc();
      } );
    }
  }

  inline void lvl( lvl::type const lvl ) noexcept {
    // make sure the lowest level that can be set is `lvl::c`
    _lvl = lvl <= lvl::m ? lvl::c : lvl;
  }

  inline bool enabled_for( lvl::type const lvl ) { return lvl <= _lvl; }

  inline chunk* accquire() noexcept {
    chunk* c{ nullptr };
    _chunk_pool.pull<backoff_strategy>(
        [&]( auto const storage ) noexcept { c = *reinterpret_cast<chunk* const*>( storage ); } );
    return c;
  }

  inline void release( chunk* c ) noexcept {
    c->reset();
    auto const idx = _chunk_pool.write_idx();
    chunk** p{ nullptr };
    for( backoff_strategy bo{}; ( p = _chunk_pool.write_ptr<chunk*>( idx ) ) == nullptr; ++bo ) bo();
    *p = c;
    _chunk_pool.write_commit( idx );
  }

  inline void recv( message_& msg ) noexcept {
    while( _message_queue.empty() ) { std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) ); }
    auto const idx = _message_queue.read_idx();
    message_ const* p{ nullptr };
    for( backoff_strategy bo{}; ( p = _message_queue.read_ptr<message_>( idx ) ) == nullptr; ++bo )
      bo();
    msg = *p;
    _message_queue.read_commit( idx );
  }

  inline void send( message_ const& msg ) noexcept {
    auto const idx = _message_queue.write_idx();
    message_* p{ nullptr };
    for( backoff_strategy bo{}; ( p = _message_queue.write_ptr<message_>( idx ) ) == nullptr; ++bo )
      bo();
    *p = msg;
    _message_queue.write_commit( idx );
  }

 public:
  static void init() noexcept {
    auto& ctx = instance();
    ctx.spawn();
  }

  static void shutdown() noexcept {
    auto& ctx = instance();
    ctx.send( message_::shutdown() );
  }

  static void use( logger* const l ) noexcept {
    auto& ctx = instance();
    ctx.send( message_::use( l ) );
  }

  static void enable( lvl::type const lvl ) noexcept {
    auto& ctx = instance();
    ctx.lvl( lvl );
  }

  template <typename... Args>
  static inline void log( lvl::type const lvl, Args&&... args ) noexcept {
    auto& ctx = actor::instance();
    if( not ctx.enabled_for( lvl ) ) { return; }
    auto chunk = ctx.accquire();
    auto s = format::stream( chunk->_buffer );
    auto m = message_::log( lvl, chunk );
    format::encode( s, lvl::to_char( lvl ), "> " );
    format::encode( s, std::forward<Args>( args )... );
    auto offset = ( s._offset + 1 ) & ( CHUNK_BUFSZ - 1 );
    chunk->_offset = offset;
    chunk->_buffer[offset - 1] = '\n';
    ctx.send( m );
  }
};

} // namespace detail

namespace {

inline void init() noexcept { detail::actor::init(); }
inline void shutdown() noexcept { detail::actor::shutdown(); }
inline void use( std::unique_ptr<logger> l ) noexcept { detail::actor::use( l.release() ); }

} // namespace

} // namespace unclassified::nanolog

using lvl = unclassified::nanolog::lvl;

namespace {

template <typename... Args>
inline void nlog( lvl::type const lvl, Args&&... args ) noexcept {
  ::unclassified::nanolog::detail::actor::log( lvl, std::forward<Args>( args )... );
}

} // namespace

#ifdef __clang__
#pragma clang diagnostic pop
#endif
