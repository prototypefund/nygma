// License: RoundRobin-1.0.0

#include <pest/pest.hxx>

#include <libioc/pattern-lexer.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "query-lexer basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace ioc;

  test( "token constructor & accessors", []( auto& expect ) {
    token tok{ token_type::LB, 0, 1 };
    expect( tok.type(), equal_to( token_type::LB ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );

  test( "scan whitespace only", []( auto& expect ) {
    std::string_view data{ "       " };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::EOS ) );
    expect( tok.offset(), equal_to( 7u ) );
    expect( tok.size(), equal_to( 0u ) );
  } );

  test( "skip comment", []( auto& expect ) {
    std::string_view data{ "# xxx" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::EOS ) );
    expect( tok.offset(), equal_to( 5u ) );
    expect( tok.size(), equal_to( 0u ) );
  } );

  test( "skip comment then scan number", []( auto& expect ) {
    std::string_view data{ "# xxx\n1" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 6u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );

  test( "scan single slash as BAD", []( auto& expect ) {
    std::string_view data{ "/" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::BAD ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );

  test( "scan empty slash pattern", []( auto& expect ) {
    std::string_view data{ "//" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::SLASH ) );
    expect( tok.offset(), equal_to( 1u ) );
    expect( tok.size(), equal_to( 0u ) );
  } );

  test( "scan /\\n/ as BAD ", []( auto& expect ) {
    std::string_view data{ "/\n/" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::BAD ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );

  test( "scan empty slash pattern with trailing chars", []( auto& expect ) {
    std::string_view data{ "//xxx" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::SLASH ) );
    expect( tok.offset(), equal_to( 1u ) );
    expect( tok.size(), equal_to( 0u ) );
  } );

  test( "scanner constructor & scan LP", []( auto& expect ) {
    std::string_view data{ "(" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::LP ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 1u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan number `0`", []( auto& expect ) {
    std::string_view data{ "0" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 1u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan number `1234`", []( auto& expect ) {
    std::string_view data{ "1234" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 4u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 4u ) );
    expect( eos.size(), equal_to( 0u ) );
  } );

  test( "scan & slice number `1234`", []( auto& expect ) {
    std::string_view data{ "1234" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::NUM ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 4u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 4u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "1234" ) );
  } );

  test( "scanner ID = `i4`", []( auto& expect ) {
    std::string_view data{ "i4" };
    scanner s{ data };
    auto const tok = s.next();
    expect( tok.type(), equal_to( token_type::ID ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 2u ) );
    auto const eos = s.next();
    expect( eos.type(), equal_to( token_type::EOS ) );
    expect( eos.offset(), equal_to( 2u ) );
    expect( eos.size(), equal_to( 0u ) );
    expect( s.slice_of( tok ), equal_to( "i4" ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
