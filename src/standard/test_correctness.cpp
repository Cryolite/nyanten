// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: GPL-3.0-or-later
// This file is part of https://github.com/Cryolite/nyanten

#include "calculator.hpp"
#include "../core.hpp"
#include <calsht.hpp>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <cstddef>


namespace {

using Nyanten::Impl_::createRNG;
using Nyanten::Impl_::createRandomPureHand;

} // namespace <anonymous>

int main(int const argc, char const * const * const argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " N" << std::endl;
    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[1]);
  std::size_t num_tests;
  iss >> num_tests;
  if (iss.fail()) {
    std::cerr << "An invalid argument." << std::endl;
    return EXIT_FAILURE;
  }

  std::mt19937 rng = createRNG();

  Calsht calculator0;
  calculator0.initialize("../shanten-number");

  Nyanten::Standard_::Calculator calculator1("map.bin");

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = createRandomPureHand(rng);
    std::uint_fast8_t const n = std::accumulate(hand.cbegin(), hand.cend(), 0u);
    std::uint_fast8_t const m = n / 3u;

    std::uint_fast8_t const shanten0 = calculator0.calc_lh(hand.data(), m);
    std::uint_fast8_t const shanten1 = calculator1(hand.cbegin(), hand.cend(), n);
    if (shanten0 != shanten1) {
      std::cerr << "Hand: ";
      for (int const tile : hand) {
        std::cerr << tile;
      }
      std::cerr << std::endl;
      std::cerr << "Mismatch: " << static_cast<unsigned>(shanten0) << " != "
                << static_cast<unsigned>(shanten1) << std::endl;
      return EXIT_FAILURE;
    }
  }
}
