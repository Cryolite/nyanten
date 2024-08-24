// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: GPL-3.0-or-later
// This file is part of https://github.com/Cryolite/nyanten

#include "../../src/common.hpp"
#include <calsht.hpp>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <random>
#include <numeric>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


namespace{

using Nyanten::Impl_::createRNG;
using Nyanten::Impl_::createRandomPureHand;

} // namespace <anonymous>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0u] << " <PATH TO shanten-number> <# OF TESTS>" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const shanten_number_path(argv[1u]);
  if (!std::filesystem::is_directory(shanten_number_path)) {
    std::cerr << shanten_number_path.string() << ": Not a directory." << std::endl;
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

  Calsht calculator;
  calculator.initialize(shanten_number_path.string());

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = createRandomPureHand(rng);
    std::uint_fast8_t const n = std::accumulate(hand.cbegin(), hand.cend(), 0u);
    std::uint_fast8_t const m = n / 3u;
    std::uint_fast8_t const volatile shanten = calculator.calc_lh(hand.data(), m);
  }

  return EXIT_SUCCESS;
}
