// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#if !defined(NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD)
#define NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD

#include <nyanten/thirteen_orphans/replacement_number.hpp>
#include <nyanten/seven_pairs/replacement_number.hpp>
#include <nyanten/standard/replacement_number.hpp>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <cstdint>


namespace Nyanten{

template<std::random_access_iterator I, std::sentinel_for<I> S>
std::uint_fast8_t calculateReplacementNumber(I first, S last)
{
  std::uint_fast8_t const n = [&]() {
    std::uint_fast8_t i = 0u;
    std::uint_fast8_t n = 0u;
    for (I iter = first; iter != last; ++iter) {
      if (*iter < 0) {
        throw std::invalid_argument("All tile counts must be non-negative.");
      }
      if (*iter > 4) {
        throw std::invalid_argument("All tile counts must be less than or equal to 4.");
      }
      ++i;
      n += *iter;
    }
    if (i != 34u) {
      throw std::invalid_argument("The number of tile types must be equal to 34.");
    }
    if (n > 14u) {
      throw std::invalid_argument("The total number of tiles must be less than or equal to 14.");
    }
    if (n % 3u == 0u) {
      throw std::invalid_argument("The total number of tiles must be congruent to 1 or 2 modulo 3.");
    }
    return n;
  }();

  std::uint_fast8_t const r0 = Nyanten::Standard_::calculateReplacementNumber(first, n);
  std::uint_fast8_t const r1 = Nyanten::SevenPairs_::calculateReplacementNumber(first, last, n);
  std::uint_fast8_t const r2
    = Nyanten::ThirteenOrphans_::calculateReplacementNumber(first, last, n);
  return std::min({r0, r1, r2});
}

template<typename R>
requires std::ranges::random_access_range<R const>
std::uint_fast8_t calculateReplacementNumber(R const &r)
{
  return Nyanten::calculateReplacementNumber(std::ranges::cbegin(r), std::ranges::cend(r));
}

} // namespace Nyanten

#endif // !defined(NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD)
