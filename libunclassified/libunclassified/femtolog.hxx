// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#endif

namespace unclassified::femtolog {

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

namespace detail {

template <typename T>
struct mailbox {
  std::mutex _mtx;
  std::condition_variable _cond;
  std::deque<T> _q;

  mailbox() = default;

  mailbox( mailbox const& ) = delete;
  mailbox& operator=( mailbox const& ) = delete;

  template <typename F>
  inline void push( F&& f ) noexcept {
    {
      std::lock_guard<std::mutex> lck{ _mtx };
      _q.emplace_back( std::forward<F>( f ) );
    }
    _cond.notify_one();
  }

  template <typename F>
  inline void pop( F& f ) noexcept {
    std::unique_lock<std::mutex> lck{ _mtx };
    _cond.wait( lck, [&]() { return ( not _q.empty() ); } );
    f = std::move( _q.front() );
    _q.pop_front();
  }
};

class aktor {
 public:
  using message = std::function<void()>;

 private:
  mailbox<message> _mailbox;
  std::thread _self;
  bool _done;

  aktor() : _done{ false } {}

  aktor( aktor const& ) = delete;
  aktor& operator=( aktor const& ) = delete;

  void run() noexcept {
    while( not _done ) {
      message m;
      _mailbox.pop( m );
      m();
    }
  }

 public:
  ~aktor() noexcept {
    send( [this] { _done = true; } );
    _self.join();
  }

  inline void send( message m ) noexcept { _mailbox.push( m ); }

  static inline std::unique_ptr<aktor> make() noexcept {
    auto act = std::unique_ptr<aktor>( new aktor );
    act->_self = std::thread( &aktor::run, act.get() );
    return act;
  }
};

} // namespace detail

class log {
  lvl::type _lvl;
  std::ostream& _os = std::clog;
  // must be last
  std::unique_ptr<detail::aktor> _self;

  log( lvl::type const lvl ) : _lvl{ lvl }, _self{ detail::aktor::make() } {}

  ~log() = default;

  inline void send( std::ostringstream& os ) noexcept {
    auto msg = os.str();
    _self->send( [this, msg = std::move( msg )]() { _os << msg; } );
  }

 public:
  static inline auto& instance() noexcept {
    static log _sink{ lvl::w };
    return ( _sink );
  }

  inline void level( lvl::type const l ) noexcept { _lvl = l <= lvl::m ? lvl::c : l; }

  template <typename... Args>
  inline void operator()( lvl::type const l, Args&&... args ) {
    if( l > _lvl ) return;
    std::ostringstream os;
    os << lvl::to_char( l ) << "> ";
    ( ( os << args ), ... );
    os << std::endl;
    send( os );
  }
};
} // namespace unclassified::femtolog

using lvl = unclassified::femtolog::lvl;

namespace {

template <typename... Args>
inline void flog( lvl::type const lvl, Args&&... args ) noexcept {
  ::unclassified::femtolog::log::instance()( lvl, std::forward<Args>( args )... );
}

} // namespace

#ifdef __clang__
#pragma clang diagnostic pop
#endif
