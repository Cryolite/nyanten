// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#if !defined(NYANTEN_SEVEN_PAIRS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
#define NYANTEN_SEVEN_PAIRS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD

#include <limits>
#include <cstdint>

namespace Nyanten::SevenPairs_{

template<typename ForwardIterator>
std::uint_fast8_t calculateReplacementNumber(
  ForwardIterator first, ForwardIterator last, std::uint_fast8_t const n)
{
  if (n != 13u && n != 14u) {
    return std::numeric_limits<std::uint_fast8_t>::max();
  }

  std::uint_fast8_t num_kinds = 0u;
  std::uint_fast8_t num_pairs = 0u;
  for (; first != last; ++first) {
    if (*first >= 1) {
      ++num_kinds;
    }
    if (*first >= 2) {
      ++num_pairs;
    }
  }

  return 7u - num_pairs + (num_kinds < 7u ? 7u - num_kinds : 0u);
}

} // namespace Nyanten::SevenPairs_

#endif // !defined(NYANTEN_SEVEN_PAIRS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
