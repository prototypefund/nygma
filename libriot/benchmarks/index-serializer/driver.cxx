// SPDX-License-Identifier: BlueOak-1.0.0

#include <argh/argh.hxx>
#include <cstdlib>
#include <exception>
#include <pest/pnch.hxx>
#include <pest/zipfian-distribution.hxx>

#include <libriot/index-compressor.hxx>
#include <libriot/index-serializer.hxx>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <sstream>

namespace {

namespace argh = emptyspace::argh;
namespace pnch = emptyspace::pnch;
namespace fs = std::filesystem;

template <typename K, typename V>
using map_type = std::map<K, V>;
using index256_type = typename riot::index_builder<std::uint32_t, map_type, 256>;
using index128_type = typename riot::index_builder<std::uint32_t, map_type, 128>;

static constexpr unsigned MINPKTSZ = 60;
static constexpr unsigned MAXPKTSZ = 1600;
static constexpr unsigned SEGMENTSZ = std::numeric_limits<unsigned>::max();

} // namespace

int main( int argc, const char** argv ) {
  argh::ArgumentParser argh( "index serializer benchmark" );
  argh::HelpFlag help( argh, "help", "guess what", { 'h', "help" } );
  argh::ValueFlag<unsigned> limit( argh, "size32", "segment size", { "limit" }, SEGMENTSZ );
  argh::ValueFlag<unsigned> unique( argh, "integer", "index entries", { "unique" }, 123 );
  argh::ValueFlag<double> skew( argh, "double", "skew for the index entry prng", { "skew" }, 0.7 );
  argh::ValueFlag<std::string> path( argh, "path", "output path", { "path" }, "/tmp/index.iv4" );
  argh::ValueFlag<std::string> compressor( argh, "uc256|svb256d1|bp128d1|bp256d1", "the compressor",
                                           { 'c' }, "uc256" );

  try {
    argh.ParseCLI( argc, argv );

    pnch::oneshot one;
    one.pin();

    std::mt19937 mt_unique{ 0x42421337 };
    std::set<std::uint32_t> unique_ips;
    std::map<std::uint32_t, std::uint32_t> ips;
    while( unique_ips.size() < argh::get( unique ) ) {
      auto const x = mt_unique();
      auto [_, is_new] = unique_ips.insert( x );
      if( is_new ) { ips.insert( { static_cast<std::uint32_t>( unique_ips.size() ), x } ); }
    }

    std::map<std::uint32_t, std::uint32_t> packet_size_distribution;
    std::mt19937 mt{ 0x2342 };
    std::normal_distribution<> size{ ( MAXPKTSZ - MINPKTSZ ) / 2.0, 180.0 };
    //std::map<std::uint32_t, std::uint32_t> histogram;
    auto const gen_packet_size = [&]() -> std::uint32_t {
      auto r = static_cast<std::uint32_t>( std::round( size( mt ) ) );
      auto x = std::max( MINPKTSZ, std::min( MAXPKTSZ, r ) );
      //++histogram[x];
      return x;
    };

    if( ips.size() <= 0 ) {
      std::clog << "no ip addresses available; exiting ..." << std::endl;
      return EXIT_FAILURE;
    }

    std::clog << "unique ips = " << ips.size() << std::endl;

    unsigned ips_lower_bound = 0;
    unsigned ips_upper_bound = static_cast<unsigned>( ips.size() - 1 );

    std::mt19937 mt_index{ 0x1337 };
    emptyspace::zipfian_int_distribution<std::uint32_t> gen_index_entry{ ips_lower_bound,
                                                                         ips_upper_bound,
                                                                         argh::get( skew ) };

    std::clog << "limit = " << argh::get( limit ) << std::endl
              << "ips_lower_bound = " << ips_lower_bound << std::endl
              << "ips_upper_bound = " << ips_upper_bound << std::endl
              << "skew = " << argh::get( skew ) << std::endl;

    std::ostringstream results;

    auto const fill_index = [&]( auto& index ) {
      // clang-format off
    unsigned packets = 0;
    one.run( "generating index", [&]() {
      for( unsigned long long total_offset = 1337; total_offset < argh::get( limit ); ) {
        auto const idx = gen_index_entry( mt_index );
        auto const ip = ips[idx];
        index.add( ip, static_cast<unsigned>( total_offset ) );
        total_offset += gen_packet_size();
        packets++;
      }
    } ).report_to( results );
      // clang-format on
      auto [min, max] = index.minmax_offset_count();
      std::clog << "distinct index keys = " << index.key_count() << std::endl
                << "fake packets ( offsets ) = " << packets << std::endl
                << "min offset entries = " << min << std::endl
                << "max offset entries = " << max << std::endl;
    };

    fs::path index_path{ argh::get( path ) };
    std::clog << "index_path = " << index_path << std::endl;

    fs::remove( index_path );

    nygma::cfile_ostream os{ index_path };
    auto comp = compressor.Get();
    if( comp == "svb256d1" ) {
      index256_type index;
      fill_index( index );
      riot::svb256d1_serializer serialize{ os };
      one.run( "serializing the index ( svb256d1 )", [&]() { index.accept( serialize, 0u ); } )
          .report_to( results );
    } else if( comp == "svq128d1" ) {
      index128_type index;
      fill_index( index );
      riot::svq128d1_serializer serialize{ os };
      one.run( "serializing the index ( svq128d1 )", [&]() { index.accept( serialize, 0u ); } )
          .report_to( results );
    } else if( comp == "bp256d1" ) {
      index256_type index;
      fill_index( index );
      riot::bp256d1_serializer serialize{ os };
      one.run( "serializing the index ( bp256d1 )", [&]() { index.accept( serialize, 0u ); } )
          .report_to( results );
    } else if( comp == "bp128d1" ) {
      index128_type index;
      fill_index( index );
      riot::bp128d1_serializer serialize{ os };
      one.run( "serializing the index ( bp128d1 )", [&]() { index.accept( serialize, 0u ); } )
          .report_to( results );
    } else {
      index256_type index;
      fill_index( index );
      riot::bytestream_serializer<256, 256, decltype( os )> serialize{ os };
      one.run( "serializing the index ( uc256 )", [&]() { index.accept( serialize, 0u ); } )
          .report_to( results );
    }

    os.sync();

    std::clog << "serialized index size = " << fs::file_size( index_path ) << std::endl;
    std::clog << results.str() << std::endl;

  } catch( argh::Help const& ) { //
    std::cerr << argh;
    return EXIT_SUCCESS;
  } catch( argh::Error const& e ) { //
    std::cerr << "error: " << e.what() << std::endl << argh;
    return EXIT_FAILURE;
  } catch( argh::ValidationError const& e ) { //
    std::cerr << e.what() << std::endl;
    argh.Help( std::cerr );
    return EXIT_FAILURE;
  } catch( std::exception const& e ) { //
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch( ... ) { std::cerr << "error: unknown exception" << std::endl; }

  //for( auto p : histogram ) {
  //  std::cout << std::setw( 2 ) << p.first << ' '
  //            << std::string( p.second / 200, '*' ) << '\n';
  //}
  return EXIT_SUCCESS;
}
