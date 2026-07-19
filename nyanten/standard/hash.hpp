// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#if !defined(NYANTEN_STANDARD_HASH_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_HASH_HPP_INCLUDE_GUARD

#include <nyanten/standard/zipai_table.hpp>
#include <nyanten/standard/shupai_table.hpp>
#include <iterator>
#include <cstdint>
#include <cassert>


namespace Nyanten::Standard_{

template<std::input_iterator I, std::sentinel_for<I> S>
std::uint_fast32_t hashShupai(I first, S last)
{
  std::uint_fast32_t h = 0u;
  {
    std::uint_fast8_t i = 0u;
    std::uint_fast8_t n = 0u;
    while (first != last) {
      assert((i < 9u));
      assert((*first <= 4));
      assert((n + *first <= 14));
      std::uint_fast8_t const c = *first++;
      n += c;
      h += shupai_table[i][n][c];
      ++i;
    }
    assert((i == 9u));
    assert((n <= 14u));
  }
  return h;
}

template<std::input_iterator I, std::sentinel_for<I> S>
std::uint_fast32_t hashZipai(I first, S last)
{
  std::uint_fast32_t h = 0u;
  {
    std::uint_fast8_t i = 0u;
    std::uint_fast8_t n = 0u;
    while (first != last) {
      assert((i < 7u));
      assert((*first <= 4));
      assert((n + *first <= 14));
      std::uint_fast8_t const c = *first++;
      n += c;
      h += zipai_table[i][n][c];
      ++i;
    }
    assert((i == 7u));
    assert((n <= 14u));
  }
  return h;
}

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_HASH_HPP_INCLUDE_GUARD)
