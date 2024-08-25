// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: GPL-3.0-or-later
// This file is part of https://github.com/Cryolite/nyanten

#include <nyanten/replacement_number.hpp>
#include "../src/common.hpp"
#include <calsht.hpp>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <tuple>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <PATH TO shanten-number> <# OF TESTS>"
      << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const shanten_number_path(argv[1]);
  if (!std::filesystem::is_directory(shanten_number_path)) {
    std::cerr << shanten_number_path.string() << ": Not a directory." << std::endl;
    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[2]);
  long long num_tests;
  iss >> num_tests;
  if (iss.fail()) {
    std::cerr << "Failed to parse the number of tests." << std::endl;
    return EXIT_FAILURE;
  }
  if (num_tests < 0) {
    std::cerr << "An invalid number of tests." << std::endl;
    return EXIT_FAILURE;
  }

  std::mt19937 rng = Nyanten::Impl_::createRNG();

  Calsht calculator0;
  calculator0.initialize(shanten_number_path.string());

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = Nyanten::Impl_::createRandomPureHand(rng);
    std::uint_fast8_t const n = std::accumulate(hand.cbegin(), hand.cend(), 0u);
    std::uint_fast8_t const m = n / 3u;

    std::uint_fast8_t shanten0;
    std::tie(shanten0, std::ignore) = calculator0(hand, m, 7);
    std::uint_fast8_t const shanten1 = Nyanten::calculateReplacementNumber(hand);
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

  return EXIT_SUCCESS;
}
