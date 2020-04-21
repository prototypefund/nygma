// SPDX-License-Identifier: UNLICENSE

#pragma once

#include <libnygma/bytestream.hxx>
#include <libriot/indexing/index-builder.hxx>

#include <deque>
#include <filesystem>
#include <sstream>
#include <thread>

namespace riot {

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

  template <typename M>
  inline void send( M&& m ) noexcept {
    _mailbox.push( std::forward<M>( m ) );
  }

  static inline std::unique_ptr<aktor> make() noexcept {
    auto act = std::unique_ptr<aktor>( new aktor );
    act->_self = std::thread( &aktor::run, act.get() );
    return act;
  }
};

} // namespace detail

class index_writer {
  // must be last ( active object pattern )
  std::unique_ptr<detail::aktor> _self;

 public:
  index_writer() : _self{ detail::aktor::make() } {}

  template <typename M>
  inline void send( M&& m ) noexcept {
    _self->send( std::forward<M>( m ) );
  }
};

class index_cycler {
  std::shared_ptr<index_writer> _w;
  std::filesystem::path _directory;
  std::filesystem::path _prefix;
  std::string _suffix;
  unsigned _count;

 public:
  index_cycler(
      std::filesystem::path const& directory,
      std::filesystem::path const& prefix,
      std::string const& suffix )
    : _w{ std::make_shared<index_writer>() },
      _directory{ directory },
      _prefix{ prefix },
      _suffix{ suffix },
      _count{ 0 } {}

  index_cycler(
      std::shared_ptr<index_writer> w,
      std::filesystem::path const& directory,
      std::filesystem::path const& prefix,
      std::string const& suffix )
    : _w{ w }, _directory{ directory }, _prefix{ prefix }, _suffix{ suffix }, _count{ 0 } {}

  auto count() const noexcept { return _count; }

  template <template <typename OS = nygma::cfile_ostream> typename S, typename I>
  void accept( std::unique_ptr<I> i, std::uint64_t const segment_offset ) noexcept {
    auto p = path();
    _count++;
    // capturing `this` is not allowed, that would introduce a race condition
    // where the worker is live but `this` may have already been destructed.
    _w->send( [p = std::move( p ), i = i.release(), segment_offset]() {
      nygma::cfile_ostream o{ p };
      S<nygma::cfile_ostream> s{ o };
      i->accept( s, segment_offset );
      delete i;
    } );
  }

  std::filesystem::path const path() noexcept {
    std::ostringstream os;
    os << '-' << std::setw( 4 ) << std::setfill( '0' ) << _count << _suffix;
    auto p = _directory / _prefix;
    p += os.str();
    return p;
  }
};

} // namespace riot
