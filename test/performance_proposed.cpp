// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#include <nyanten/replacement_number.hpp>
#include "../src/common.hpp"
#include <sstream>
#include <iostream>
#include <random>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


int main(int const argc, char const * const * const argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0u] << " <# OF TESTS>" << std::endl;
    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[1u]);
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

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = Nyanten::Impl_::createRandomPureHand(rng);
    std::uint_fast8_t const n = std::accumulate(hand.cbegin(), hand.cend(), 0u);
    std::uint_fast8_t const volatile replacement_number
      = Nyanten::calculateReplacementNumber(hand.cbegin(), hand.cend());
  }

  return EXIT_SUCCESS;
}
