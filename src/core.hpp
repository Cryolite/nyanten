#if !defined(NYANTEN_CORE_HPP_INCLUDE_GUARD)
#define NYANTEN_CORE_HPP_INCLUDE_GUARD

#include <random>
#include <algorithm>
#include <vector>
#include <array>
#include <cstdint>


namespace Nyanten::Impl_ {

inline std::mt19937 createRNG()
{
  std::random_device rd;
  std::vector<std::uint_least32_t> seed(std::mt19937::state_size);
  std::generate(seed.begin(), seed.end(), std::ref(rd));
  std::seed_seq ss(seed.begin(), seed.end());
  return std::mt19937(ss);
}

inline std::vector<int> createRandomPureHand(std::mt19937 &rng)
{
  std::vector<int> paishan(136u);
  std::iota(paishan.begin(), paishan.end(), 0);
  std::shuffle(paishan.begin(), paishan.end(), rng);

  std::uint_fast8_t const t = [&]() {
    std::uniform_int_distribution<std::uint_fast8_t> dist(0u, 9u);
    return std::array<std::uint_fast8_t, 10u>{1u, 2u, 4u, 5u, 7u, 8u, 10u, 11u, 13u, 14u}[dist(rng)];
  }();
  std::vector<int> hand136(paishan.cbegin(), paishan.cbegin() + t);
  std::vector<int> result(34u, 0);
  for (int const tile : hand136) {
    ++result[tile / 4];
  }
  return result;
}

} // namespace Nyanten::Impl_

#endif // !defined(NYANTEN_CORE_HPP_INCLUDE_GUARD)
