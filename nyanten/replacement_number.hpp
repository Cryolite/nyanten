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
#include <cstdint>


namespace Nyanten{

template<typename ForwardIterator>
std::uint_fast8_t calculateReplacementNumber(ForwardIterator first, ForwardIterator last)
{
  std::uint_fast8_t const n = [&]() {
    std::uint_fast8_t i = 0u;
    std::uint_fast8_t n = 0u;
    for (ForwardIterator iter = first; iter != last; ++iter) {
      if (*iter < 0) {
        throw std::invalid_argument("An invalid hand.");
      }
      if (*iter > 4) {
        throw std::invalid_argument("An invalid hand.");
      }
      ++i;
      n += *iter;
    }
    if (i != 34u) {
      throw std::invalid_argument("An invalid hand.");
    }
    if (n > 14u) {
      throw std::invalid_argument("An invalid hand.");
    }
    if (n % 3u == 0u) {
      throw std::invalid_argument("An invalid hand.");
    }
    return n;
  }();

  std::uint_fast8_t const r0 = Nyanten::Standard_::calculateReplacementNumber(first, last, n);
  std::uint_fast8_t const r1 = Nyanten::SevenPairs_::calculateReplacementNumber(first, last, n);
  std::uint_fast8_t const r2
    = Nyanten::ThirteenOrphans_::calculateReplacementNumber(first, last, n);
  return std::min({r0, r1, r2});
}

template<typename ForwardRange>
std::uint_fast8_t calculateReplacementNumber(ForwardRange const &r)
{
  return Nyanten::calculateReplacementNumber(std::cbegin(r), std::cend(r));
}

} // namespace Nyanten

#endif // !defined(NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD)
