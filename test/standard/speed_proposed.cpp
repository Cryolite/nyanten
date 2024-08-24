// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#include <nyanten/standard/calculator.hpp>
#include "../../src/common.hpp"
#include <sstream>
#include <iostream>
#include <random>
#include <numeric>
#include <vector>
#include <cstdint>
#include <cstddef>


namespace{

using Nyanten::Impl_::createRNG;
using Nyanten::Impl_::createRandomPureHand;
using Nyanten::Standard_::Calculator;

} // namespace <anonymous>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0u] << " <PATH TO map.bin> <# OF TESTS>" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const map_path(argv[1u]);
  if (!std::filesystem::is_regular_file(map_path) && !std::filesystem::is_symlink(map_path)) {
    std::cerr << map_path.string() << ": Not a file." << std::endl;
    return EXIT_FAILURE;
  }

  std::istringstream iss(argv[2u]);
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

  Calculator calculator(map_path);

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = createRandomPureHand(rng);
    std::uint_fast8_t const n = std::accumulate(hand.cbegin(), hand.cend(), 0u);
    std::uint_fast8_t const volatile replacement_number
      = calculator(hand.cbegin(), hand.cend(), n);
  }

  return EXIT_SUCCESS;
}
