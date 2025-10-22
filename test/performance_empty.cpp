// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#include "../src/common.hpp"
#include <sstream>
#include <iostream>
#include <random>
#include <numeric>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


namespace{

using Nyanten::Impl_::createRNG;
using Nyanten::Impl_::createRandomPureHand;

} // namespace <anonymous>

int main(int const argc, char const * const * const argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <# OF TESTS>" << std::endl;
    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[1]);
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

  std::mt19937 rng = createRNG();

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::array<std::uint_fast8_t, 34u> const hand = createRandomPureHand(rng);
  }

  return EXIT_SUCCESS;
}
