#include "hash.hpp"
#include "zipai_table.hpp"
#include "shupai_table.hpp"
#include "core.hpp"
#include <fstream>
#include <ios>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <cassert>


namespace {

using Nyanten::Standard_::shupai_size;
using Nyanten::Standard_::zipai_size;
using Nyanten::Standard_::Map;

std::array<std::array<std::uint_fast8_t, 3u>, 8u> s_table = {{
  {{0u, 0u, 0u}},
  {{0u, 0u, 1u}},
  {{0u, 0u, 2u}},
  {{0u, 1u, 0u}},
  {{0u, 1u, 1u}},
  {{1u, 0u, 0u}},
  {{1u, 0u, 1u}},
  {{1u, 0u, 2u}},
}};
std::array<std::uint_fast8_t, 8u> m_table{0u, 1u, 2u, 1u, 2u, 0u, 1u, 2u};
std::array<std::uint_fast8_t, 8u> n_table{0u, 1u, 2u, 3u, 4u, 2u, 3u, 4u};

template<std::size_t N>
std::uint_fast8_t getHandDistance(
  std::array<std::uint_fast8_t, N> const &hand,
  std::array<std::uint_fast8_t, N> const &winning_hand)
{
  std::uint_fast8_t distance = 0u;
  for (std::uint_fast8_t i = 0u; i < N; ++i) {
    if (winning_hand[i] > hand[i]) {
      distance += winning_hand[i] - hand[i];
    }
  }
  return distance;
}

void getShupaiReplacementNumber(
  std::array<std::uint_fast8_t, 9u> const &hand,
  std::uint_fast8_t const m,
  std::uint_fast8_t const h,
  std::uint_fast8_t const i,
  std::uint_fast8_t const mm,
  std::uint_fast8_t const hh,
  std::uint_fast8_t const x,
  std::uint_fast8_t const y,
  std::array<std::uint_fast8_t, 9u> &winning_hand,
  std::uint_fast8_t &replacement_number)
{
  assert((m <= 4u));
  assert((h <= 1u));
  assert((i <= 9u));
  assert((mm <= m));
  assert((hh <= h));
  assert((x <= 4u));
  assert((y <= 2u));

  if (i == 9u) {
    if (mm == m && hh == h) {
      std::uint_fast8_t const distance = getHandDistance(hand, winning_hand);
      replacement_number = std::min<std::uint_fast8_t>(replacement_number, distance);
    }
    return;
  }

  for (std::uint_fast8_t s = 0u; s < s_table.size(); ++s) {
    if (mm + m_table[s] > m) {
      continue;
    }
    if (hh + s_table[s][0u] > h) {
      continue;
    }
    if (i + 2u >= 9u && s_table[s][2u] > 0u) {
      continue;
    }
    if (x + n_table[s] > 4u) {
      continue;
    }

    if (i + 2u < 9u) {
      winning_hand[i] += s_table[s][2u];
      winning_hand[i + 1u] += s_table[s][2u];
      winning_hand[i + 2u] += s_table[s][2u];
    }
    winning_hand[i] += s_table[s][1u] * 3u;
    winning_hand[i] += s_table[s][0u] * 2u;
    getShupaiReplacementNumber(
      hand,
      m,
      h,
      i + 1u,
      mm + m_table[s],
      hh + s_table[s][0u],
      y + s_table[s][2u],
      s_table[s][2u],
      winning_hand,
      replacement_number);
    winning_hand[i] -= s_table[s][0u] * 2u;
    winning_hand[i] -= s_table[s][1u] * 3u;
    if (i + 2u < 9u) {
      winning_hand[i + 2u] -= s_table[s][2u];
      winning_hand[i + 1u] -= s_table[s][2u];
      winning_hand[i] -= s_table[s][2u];
    }
  }
}

void getZipaiReplacementNumber(
  std::array<std::uint_fast8_t, 7u> const &hand,
  std::uint_fast8_t const m,
  std::uint_fast8_t const h,
  std::uint_fast8_t const i,
  std::uint_fast8_t const mm,
  std::uint_fast8_t const hh,
  std::array<std::uint_fast8_t, 7u> &winning_hand,
  std::uint_fast8_t &replacement_number)
{
  assert((m <= 4u));
  assert((h <= 1u));
  assert((i <= 7u));
  assert((mm <= m));
  assert((hh <= h));

  if (i == 7u) {
    if (mm == m && hh == h) {
      std::uint_fast8_t const distance = getHandDistance(hand, winning_hand);
      replacement_number = std::min<std::uint_fast8_t>(replacement_number, distance);
    }
    return;
  }

  for (std::uint_fast8_t s = 0u; s < s_table.size(); ++s) {
    if (mm + m_table[s] > m) {
      continue;
    }
    if (hh + s_table[s][0u] > h) {
      continue;
    }
    if (s_table[s][2u] > 0u) {
      continue;
    }

    winning_hand[i] += s_table[s][1u] * 3u;
    winning_hand[i] += s_table[s][0u] * 2u;
    getZipaiReplacementNumber(
      hand,
      m,
      h,
      i + 1u,
      mm + m_table[s],
      hh + s_table[s][0u],
      winning_hand,
      replacement_number);
    winning_hand[i] -= s_table[s][0u] * 2u;
    winning_hand[i] -= s_table[s][1u] * 3u;
  }
}

template<std::size_t N>
std::uint64_t packReplacementNumbers(std::array<std::uint_fast8_t, N> const &hand)
{
  static_assert(N == 7u || N == 9u);

  std::uint64_t pack = 0u;
  {
    std::uint64_t base = 1u;
    for (std::uint_fast8_t h = 0u; h <= 1u; ++h) {
      for (std::uint_fast8_t m = 0u; m <= 4u; ++m) {
        std::uint_fast8_t replacement_number = 9u;
        if constexpr (N == 9u) {
          std::array<std::uint_fast8_t, 9u> winning_hand{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
          getShupaiReplacementNumber(
            hand, m, h, 0u, 0u, 0u, 0u, 0u, winning_hand, replacement_number);
        }
        else {
          std::array<std::uint_fast8_t, 7u> winning_hand{0u, 0u, 0u, 0u, 0u, 0u, 0u};
          getZipaiReplacementNumber(
            hand, m, h, 0u, 0u, 0u, winning_hand, replacement_number);
        }
        pack += replacement_number * base;
        base *= 10u;
      }
    }
  }
  return pack;
}

template<std::size_t N>
void createEntry(std::array<std::uint_fast8_t, N> const &hand, Map &map)
{
  static_assert(N == 7u || N == 9u);

  std::uint_fast32_t const h = [&]() {
    if constexpr (N == 9u) {
      return Nyanten::Standard_::hashShupai(hand.cbegin(), hand.cend());
    }
    else {
      return Nyanten::Standard_::hashZipai(hand.cbegin(), hand.cend());
    }
  }();

  std::uint64_t const entry = packReplacementNumbers(hand);

  map[h] = entry;
}

template<std::size_t N>
void buildMap(
  std::array<std::uint_fast8_t, N> &hand,
  std::uint_fast8_t const i,
  std::uint_fast8_t const n,
  Map &map)
{
  static_assert(N == 7u || N == 9u);
  assert((i <= N));
  assert((n <= 14u));

  if (i == N) {
    createEntry(hand, map);
    return;
  }

  for (std::uint_fast8_t c = 0u; c <= 4u; ++c) {
    if (n + c > 14u) {
      break;
    }

    hand[i] = c;
    buildMap(hand, i + 1u, n + c, map);
    hand[i] = 0u;
  }
}

} // namespace <anonymous>

int main()
{
  {
    Map shupai_map;
    shupai_map.resize(shupai_size);
    std::array<std::uint_fast8_t, 9u> hand = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    buildMap(hand, 0u, 0u, shupai_map);

    std::ofstream ofs("src/standard/shupai_map.bin", std::ios::binary);
    ofs.write(
      reinterpret_cast<char const *>(shupai_map.data()), shupai_map.size() * sizeof(std::uint64_t));
    ofs.flush();
  }

  {
    Map zipai_map;
    zipai_map.resize(zipai_size);
    std::array<std::uint_fast8_t, 7u> hand = {0u, 0u, 0u, 0u, 0u, 0u, 0u};
    buildMap(hand, 0u, 0u, zipai_map);

    std::ofstream ofs("src/standard/zipai_map.bin", std::ios::binary);
    ofs.write(
      reinterpret_cast<char const *>(zipai_map.data()), zipai_map.size() * sizeof(std::uint64_t));
    ofs.flush();
  }
}
