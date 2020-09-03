// SPDX-License-Identifier: BlueOak-1.0.0

#pragma once

#include <chrono>
#include <thread>

#if defined( __x86_64__ )
#  include <xmmintrin.h>
#endif

namespace unclassified::backoff_strategy {
namespace {
#if defined( __x86_64__ )
inline void pause() noexcept { _mm_pause(); }
#else
inlie void pause() noexept {}
#endif
} // namespace

struct backoff2 {
  static constexpr int k_fast_path_thresold = 16;

  int _count{ 0 };

  inline void operator()() noexcept {
    if( _count < k_fast_path_thresold ) {
      pause();
    } else {
      std::this_thread::yield();
      _count = 0;
    }
  }

  inline void operator++() noexcept { _count++; }
};

struct backoff3 {
  static constexpr int k_fast_path_threshold = 16;
  static constexpr int k_slow_path_threshold = 32;
  static constexpr int k_reset_threshold = 64;

  int _count{ 0 };

  inline void operator()() noexcept {
    if( _count < k_fast_path_threshold ) {
      pause();
    } else if( _count < k_slow_path_threshold ) {
      std::this_thread::yield();
    } else {
      std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
      if( _count > k_reset_threshold ) _count = 0;
    }
  }

  inline void operator++() noexcept { _count++; }
};

} // namespace unclassified::backoff_strategy
