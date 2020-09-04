// License: RoundRobin-1.0.0

#include <pest/pest.hxx>

#include <libioc/pattern-parser.hxx>

#include <string_view>

namespace {

emptyspace::pest::suite basic( "pattern-parser basic suite", []( auto& test ) {
  using namespace emptyspace::pest;
  using namespace ioc;

  test( "token constructor & accessors", []( auto& expect ) {
    token tok{ token_type::LB, 0, 1 };
    expect( tok.type(), equal_to( token_type::LB ) );
    expect( tok.offset(), equal_to( 0u ) );
    expect( tok.size(), equal_to( 1u ) );
  } );

  test( "parse pattern", []( auto& expect ) {
    std::string_view const pattern{ "1:" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
  } );

  test( "parse comment then pattern", []( auto& expect ) {
    std::string_view const pattern{ "# comment\n1:" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
  } );

  test( "parse pure pattern", []( auto& expect ) {
    std::string_view const pattern{ "1:||" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "pure" ) );
  } );

  test( "parse regexp pattern", []( auto& expect ) {
    std::string_view const pattern{ "1://" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "regexp" ) );
  } );

  test( "parse ipaddr pattern", []( auto& expect ) {
    std::string_view const pattern{ "1:\\\\" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "ipaddr" ) );
  } );

  test( "parse domain pattern", []( auto& expect ) {
    std::string_view const pattern{ "1:~~" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "regexp-domain" ) );
  } );

  test( "parse pure domain pattern", []( auto& expect ) {
    std::string_view const pattern{ "1:''" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "pure-domain" ) );
  } );

  test( "parse pure domain: icanhascheez.com", []( auto& expect ) {
    std::string_view const pattern{ "1:'icanhascheez.com'" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "icanhascheez.com" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "pure-domain" ) );
  } );

  test( "parse regexp pattern with annotation: i", []( auto& expect ) {
    std::string_view const pattern{ "1://i" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "regexp" ) );
    expect( patterns[0]->second["i"], equal_to( "true" ) );
  } );

  test( "parse regexp pattern with annotation: i min=23 max=42", []( auto& expect ) {
    std::string_view const pattern{ "1:/abc/i min=23 max=42" };
    auto patterns = ioc::parse( pattern );
    expect( not patterns->empty() );
    expect( patterns[0]->first, equal_to( 1u ) );
    expect( patterns[0]->second["pattern"], equal_to( "abc" ) );
    expect( patterns[0]->second["pattern_type"], equal_to( "regexp" ) );
    expect( patterns[0]->second["i"], equal_to( "true" ) );
    expect( patterns[0]->second["min"], equal_to( "23" ) );
    expect( patterns[0]->second["max"], equal_to( "42" ) );
  } );
} );

}

int main() {
  basic( std::clog );
  return EXIT_SUCCESS;
}
