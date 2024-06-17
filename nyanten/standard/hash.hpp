// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#if !defined(NYANTEN_STANDARD_HASH_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_HASH_HPP_INCLUDE_GUARD

#include "zipai_table.hpp"
#include "shupai_table.hpp"
#include <cstdint>
#include <cassert>


namespace Nyanten::Standard_ {

template<typename ForwardIterator>
std::uint_fast32_t hashShupai(ForwardIterator first, ForwardIterator last)
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

template<typename ForwardIterator>
std::uint_fast32_t hashZipai(ForwardIterator first, ForwardIterator last)
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
