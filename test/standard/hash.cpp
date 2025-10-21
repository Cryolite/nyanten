// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#include <nyanten/standard/hash.hpp>
#include <nyanten/standard/zipai_table.hpp>
#include <nyanten/standard/shupai_table.hpp>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cassert>


namespace{

using Nyanten::Standard_::shupai_size;
using Nyanten::Standard_::zipai_size;
using Nyanten::Standard_::hashShupai;
using Nyanten::Standard_::hashZipai;

template<std::size_t N>
void testHash(std::array<std::uint_fast8_t, N> const &hand, std::vector<std::uint_fast8_t> &check)
{
  std::uint_fast32_t const h = [&]() {
    if constexpr (N == 9u) {
      std::uint_fast32_t const h = hashShupai(hand.cbegin(), hand.cend());
      if (h >= shupai_size) {
        throw std::logic_error("Out of range.");
      }
      return h;
    }
    std::uint_fast32_t const h = hashZipai(hand.cbegin(), hand.cend());
    if (h >= zipai_size) {
      throw std::logic_error("Out of range.");
    }
    return h;
  }();

  if (check[h] != 0u) {
    throw std::logic_error("Collision.");
  }
  check[h] = 1u;
}

template<std::size_t N>
void buildHand(
  std::uint_fast8_t const i,
  std::array<std::uint_fast8_t, N> &hand,
  std::uint_fast8_t n,
  std::vector<std::uint_fast8_t> &check)
{
  static_assert(N == 9u || N == 7u);
  assert((i <= N));
  assert((n <= 14u));

  if (i == N) {
    testHash(hand, check);
    return;
  }

  assert((hand[i] == 0u));

  for (std::uint_fast8_t c = 0u; c <= 4u; ++c) {
    if (n + c > 14u) {
      break;
    }
    hand[i] = c;
    buildHand(i + 1u, hand, n + c, check);
    hand[i] = 0u;
  }
}

} // namespace <anonymous>

int main()
{

  {
    std::array<std::uint_fast8_t, 9u> hand{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    std::vector<std::uint_fast8_t> check(shupai_size, 0u);
    buildHand(0u, hand, 0u, check);
    for (auto const c : check) {
      if (c == 0u) {
        throw std::logic_error("A logic error.");
      }
    }
  }

  {
    std::array<std::uint_fast8_t, 7u> hand{0u, 0u, 0u, 0u, 0u, 0u, 0u};
    std::vector<std::uint_fast8_t> check(zipai_size, 0u);
    buildHand(0u, hand, 0u, check);
    for (auto const c : check) {
      if (c == 0u) {
        throw std::logic_error("A logic error.");
      }
    }
  }

  return EXIT_SUCCESS;
}
