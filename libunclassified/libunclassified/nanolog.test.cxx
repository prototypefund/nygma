// SPDX-License-Identifier: BlueOak-1.0.0

#include <pest/pest.hxx>

#include <libunclassified/nanolog.hxx>

#include <filesystem>
#include <future>

namespace {

emptyspace::pest::suite basic( "nanolog basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace unclassified::nanolog;

  test( "nanolog accquire chunk", []( auto& expect ) {
    auto& ctx = detail::actor::instance();
    auto chunk = ctx.accquire();
    expect( chunk != nullptr, equal_to( true ) );
    ctx.release( chunk );
  } );

  test( "nanolog send fake init", []( auto& expect ) {
    auto& ctx = detail::actor::instance();
    auto logger = std::make_unique<fileio>( "/dev/null" );
    ctx.send( detail::message_::use( logger.get() ) );
    detail::message_ msg;
    ctx.recv( msg );
    expect( msg._type, equal_to( detail::message_type::use ) );
    expect( msg._p._use._logger, equal_to( logger.get() ) );
  } );

  test( "nanolog send fake shutdown", []( auto& expect ) {
    auto& ctx = detail::actor::instance();
    ctx.send( detail::message_::shutdown() );
    detail::message_ msg;
    ctx.recv( msg );
    expect( msg._type, equal_to( detail::message_type::shutdown ) );
  } );

  test( "nanolog send fake log", []( auto& expect ) {
    auto& ctx = detail::actor::instance();
    auto chunk = ctx.accquire();
    ctx.send( detail::message_::log( lvl::i, chunk ) );
    detail::message_ msg;
    ctx.recv( msg );
    ctx.release( chunk );
    expect( msg._type, equal_to( detail::message_type::log ) );
    expect( msg._p._log._chunk, equal_to( chunk ) );
  } );

  test( "nanolog check timestamp", []( auto& expect ) {
    auto& ctx = detail::actor::instance();
    auto orig = detail::message_::shutdown();
    ctx.send( orig );
    detail::message_ msg;
    ctx.recv( msg );
    expect( msg._type, equal_to( detail::message_type::shutdown ) );
    expect( msg._stamp, equal_to( orig._stamp ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
