// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <libriot/index-resultset.hxx>
#include <libriot/index-view.hxx>
#include <libriot/query-ast.hxx>

#include <unordered_map>

namespace riot {

struct environment {

  using indices_type = std::unordered_map<std::string, index_view_handle>;
  using resultset_type = resultset_forward_type;

  struct builder {
    indices_type _indices;

    builder() {}

    // non-owning wrapper arround data; make sure `data` outlives the environment
    builder& add( std::string name, bytestring_view const data ) {
      _indices.emplace( std::move( name ), index_view_handle{ data } );
      return *this;
    }

    builder& add( std::string name, std::filesystem::path const& p ) {
      _indices.emplace( std::move( name ), index_view_handle{ p } );
      return *this;
    }

    environment build() noexcept { return environment( std::move( _indices ) ); }
  };

  indices_type _indices;

  template <typename Map>
  explicit environment( Map&& map ) : _indices{ std::forward<Map>( map ) } {}

  //--query-evaluator-implementation------------------------------------------

  template <typename T>
  resultset_type eval( T const& t ) const {
    return t->eval( *this );
  }

  resultset_type operator()( ident const& ) const { return resultset_type::none(); }
  resultset_type operator()( number const& ) const { return resultset_type::none(); }
  resultset_type operator()( ipv4 const& ) const { return resultset_type::none(); }
  resultset_type operator()( ipv6 const& ) const { return resultset_type::none(); }
  resultset_type operator()( binary const& b ) const {
    switch( b._op ) {
      case binop::UNION: return eval( b._a ) + eval( b._b );
      case binop::COMPLEMENT: return eval( b._a ) - eval( b._b );
      case binop::INTERSECTION: return eval( b._a ) & eval( b._b );
    }
  }
  resultset_type operator()( query const& q ) const {
    switch( q._method ) {
      case query_method::COMBINED: throw std::runtime_error( "query_method::COMBINED unimplemented" );
      case query_method::REVERSE: throw std::runtime_error( "query_method::REVERSE unimplemented" );
      case query_method::FORWARD: {
        auto const name = q._name->eval<kind::ID>( []( auto const& id ) { return id._name; } );
        return q._what->eval( overloaded{
            [&]( number const& n ) {
              auto const it = _indices.find( name );
              if( it == _indices.end() ) { return resultset_type::none(); }
              return it->second->lookup_forward_32( static_cast<std::uint32_t>( n._value ) );
            },
            [&]( ipv4 const& i4 ) {
              auto const it = _indices.find( name );
              if( it == _indices.end() ) { return resultset_type::none(); }
              return it->second->lookup_forward_32( static_cast<std::uint32_t>( i4._value ) );
            },
            [&]( ipv6 const& i6 ) {
              auto const it = _indices.find( name );
              if( it == _indices.end() ) { return resultset_type::none(); }
              return it->second->lookup_forward_128( i6._value );
            },
            []( auto const& ) { return resultset_type::none(); },
        } );
      }
    }
    return resultset_type::none();
  }
};

} // namespace riot
