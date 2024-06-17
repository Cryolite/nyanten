// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#include "../core.hpp"
#include <sstream>
#include <iostream>
#include <random>
#include <numeric>
#include <vector>
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

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = createRandomPureHand(rng);
  }
}
