#include "calculator.hpp"
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
using Nyanten::Standard_::Calculator;

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

  Calculator calculator("src/standard/map.bin");

  for (std::size_t i = 0u; i < num_tests; ++i) {
    std::vector<int> const hand = createRandomPureHand(rng);
    std::uint_fast8_t const shanten = calculator(hand.cbegin(), hand.cend());
  }
}
