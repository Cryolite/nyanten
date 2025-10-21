// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#include "common.hpp"
#include <nyanten/standard/hash.hpp>
#include <nyanten/standard/zipai_table.hpp>
#include <nyanten/standard/shupai_table.hpp>
#include <nyanten/standard/core.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cassert>


namespace{

using Nyanten::Standard_::shupai_size;
using Nyanten::Standard_::zipai_size;
using Nyanten::Standard_::MapValue;
using Map = std::vector<MapValue>;
using Nyanten::Standard_::Unpack;
using Nyanten::Standard_::pack;

constexpr std::array<std::array<std::uint_fast8_t, 3u>, 8u> d_table = {{
  {{0u, 0u, 0u}},
  {{0u, 0u, 1u}},
  {{0u, 1u, 0u}},
  {{1u, 0u, 0u}},
  {{1u, 0u, 1u}},
  {{1u, 1u, 0u}},
  {{2u, 0u, 0u}},
  {{2u, 0u, 1u}},
}};
constexpr std::array<std::uint_fast8_t, 8u> m_table{
  d_table[0u][0u] + d_table[0u][1u],
  d_table[1u][0u] + d_table[1u][1u],
  d_table[2u][0u] + d_table[2u][1u],
  d_table[3u][0u] + d_table[3u][1u],
  d_table[4u][0u] + d_table[4u][1u],
  d_table[5u][0u] + d_table[5u][1u],
  d_table[6u][0u] + d_table[6u][1u],
  d_table[7u][0u] + d_table[7u][1u],
};
constexpr std::array<std::uint_fast8_t, 8u> n_table{
  d_table[0u][0u] + 3u * d_table[0u][1u] + 2u * d_table[0u][2u],
  d_table[1u][0u] + 3u * d_table[1u][1u] + 2u * d_table[1u][2u],
  d_table[2u][0u] + 3u * d_table[2u][1u] + 2u * d_table[2u][2u],
  d_table[3u][0u] + 3u * d_table[3u][1u] + 2u * d_table[3u][2u],
  d_table[4u][0u] + 3u * d_table[4u][1u] + 2u * d_table[4u][2u],
  d_table[5u][0u] + 3u * d_table[5u][1u] + 2u * d_table[5u][2u],
  d_table[6u][0u] + 3u * d_table[6u][1u] + 2u * d_table[6u][2u],
  d_table[7u][0u] + 3u * d_table[7u][1u] + 2u * d_table[7u][2u],
};

template<std::size_t N>
std::uint_fast8_t getHandDistance(
  std::array<std::uint_fast8_t, N> const &target_hand,
  std::array<std::uint_fast8_t, N> const &hand)
{
  std::uint_fast8_t distance = 0u;
  for (std::uint_fast8_t i = 0u; i < N; ++i) {
    if (target_hand[i] > hand[i]) {
      distance += target_hand[i] - hand[i];
    }
  }
  return distance;
}

std::uint_fast8_t getShupaiReplacementNumber(
  std::array<std::uint_fast8_t, 9u> const &hand,
  std::uint_fast8_t const m,
  std::uint_fast8_t const h,
  std::uint_fast8_t const i,
  std::uint_fast8_t const mm,
  std::uint_fast8_t const hh,
  std::array<std::uint_fast8_t, 9u> &target_hand,
  std::uint_fast8_t upper_bound)
{
  assert((m <= 4u));
  assert((h <= 1u));
  assert((i <= 9u));
  assert((mm <= m));
  assert((hh <= h));

  if (i == 9u) {
    if (mm == m && hh == h) {
      upper_bound = std::min<std::uint_fast8_t>(upper_bound, getHandDistance(target_hand, hand));
    }
    return upper_bound;
  }

  for (std::uint_fast8_t j = 0u; j < d_table.size(); ++j) {
    if (mm + m_table[j] > m) {
      continue;
    }
    if (hh + d_table[j][2u] > h) {
      continue;
    }
    if (i + 2u >= 9u && d_table[j][0u] >= 1u) {
      continue;
    }
    if (target_hand[i] + n_table[j] > 4u) {
      continue;
    }

    target_hand[i] += n_table[j];
    if (i + 2u < 9u) {
      target_hand[i + 1u] += d_table[j][0u];
      target_hand[i + 2u] += d_table[j][0u];
    }
    std::uint_fast8_t const lower_bound = getHandDistance(target_hand, hand);
    if (lower_bound < upper_bound) {
      std::uint_fast8_t const tmp = getShupaiReplacementNumber(
        hand,
        m,
        h,
        i + 1u,
        mm + m_table[j],
        hh + d_table[j][2u],
        target_hand,
        upper_bound);
      upper_bound = std::min<std::uint_fast8_t>(upper_bound, tmp);
    }
    if (i + 2u < 9u) {
      target_hand[i + 2u] -= d_table[j][0u];
      target_hand[i + 1u] -= d_table[j][0u];
    }
    target_hand[i] -= n_table[j];
  }

  return upper_bound;
}

std::uint_fast8_t getZipaiReplacementNumber(
  std::array<std::uint_fast8_t, 7u> const &hand,
  std::uint_fast8_t const m,
  std::uint_fast8_t const h,
  std::uint_fast8_t const i,
  std::uint_fast8_t const mm,
  std::uint_fast8_t const hh,
  std::array<std::uint_fast8_t, 7u> &target_hand,
  std::uint_fast8_t upper_bound)
{
  assert((m <= 4u));
  assert((h <= 1u));
  assert((i <= 7u));
  assert((mm <= m));
  assert((hh <= h));

  if (i == 7u) {
    if (mm == m && hh == h) {
      upper_bound = std::min<std::uint_fast8_t>(upper_bound, getHandDistance(target_hand, hand));
    }
    return upper_bound;
  }

  for (std::uint_fast8_t j = 0u; j < d_table.size(); ++j) {
    if (mm + m_table[j] > m) {
      continue;
    }
    if (hh + d_table[j][2u] > h) {
      continue;
    }
    if (d_table[j][0u] > 0u) {
      continue;
    }

    target_hand[i] += n_table[j];
    std::uint_fast8_t const lower_bound = getHandDistance(target_hand, hand);
    if (lower_bound < upper_bound) {
      std::uint_fast8_t const tmp = getZipaiReplacementNumber(
        hand,
        m,
        h,
        i + 1u,
        mm + m_table[j],
        hh + d_table[j][2u],
        target_hand,
        upper_bound);
      upper_bound = std::min<std::uint_fast8_t>(upper_bound, tmp);
    }
    target_hand[i] -= n_table[j];
  }

  return upper_bound;
}

template<std::size_t N>
MapValue packReplacementNumbers(std::array<std::uint_fast8_t, N> const &hand)
{
  static_assert(N == 7u || N == 9u);

  Unpack unpack;
  for (std::uint_fast8_t h = 0u; h <= 1u; ++h) {
    for (std::uint_fast8_t m = 0u; m <= 4u; ++m) {
      if (m == 0u && h == 0u) {
        continue;
      }
      if constexpr (N == 9u) {
        std::array<std::uint_fast8_t, 9u> target_hand{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
        unpack[5u * h + m - 1u] = getShupaiReplacementNumber(hand, m, h, 0u, 0u, 0u, target_hand, UINT_FAST8_MAX);
      }
      else {
        std::array<std::uint_fast8_t, 7u> target_hand{0u, 0u, 0u, 0u, 0u, 0u, 0u};
        unpack[5u * h + m - 1u] = getZipaiReplacementNumber(hand, m, h, 0u, 0u, 0u, target_hand, UINT_FAST8_MAX);
      }
    }
  }

  return pack(unpack);
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

  MapValue const entry = packReplacementNumbers(hand);

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

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <PATH TO SHUPAI MAP> <PATH TO ZIPAI MAP>" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const shupai_map_path(argv[1]);
  std::filesystem::path const zipai_map_path(argv[2]);

  {
    Map shupai_map;
    shupai_map.resize(shupai_size);
    std::array<std::uint_fast8_t, 9u> hand = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    buildMap(hand, 0u, 0u, shupai_map);

    std::ofstream ofs(shupai_map_path);
    if (!ofs) {
      throw std::runtime_error("Failed to create the shupai map file.");
    }

    ofs << "// Copyright (c) 2024 Cryolite. All rights reserved.\n";
    ofs << "// SPDX-License-Identifier: MIT\n";
    ofs << "// This file is part of https://github.com/Cryolite/nyanten.\n";
    ofs << '\n';
    ofs << "#if !defined(NYANTEN_SRC_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_SRC_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD\n";
    ofs << '\n';
    ofs << "#include <nyanten/standard/shupai_table.hpp>\n";
    ofs << "#include <nyanten/standard/core.hpp>\n";
    ofs << '\n';
    ofs << '\n';
    ofs << "namespace Nyanten::Standard_{\n";
    ofs << '\n';
    ofs << "inline constexpr std::array<MapValue, Nyanten::Standard_::shupai_size> shupai_map{";
    for (MapValue const &pack : shupai_map) {
      ofs << pack << "u,";
    }
    ofs << "};\n";
    ofs << '\n';
    ofs << "} // namespace Nyanten::Standard_\n";
    ofs << '\n';
    ofs << "#endif // !defined(NYANTEN_SRC_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD)\n";
  }

  {
    Map zipai_map;
    zipai_map.resize(zipai_size);
    std::array<std::uint_fast8_t, 7u> hand = {0u, 0u, 0u, 0u, 0u, 0u, 0u};
    buildMap(hand, 0u, 0u, zipai_map);

    std::ofstream ofs(zipai_map_path);
    if (!ofs) {
      throw std::runtime_error("Failed to create the zipai map file.");
    }

    ofs << "// Copyright (c) 2024 Cryolite. All rights reserved.\n";
    ofs << "// SPDX-License-Identifier: MIT\n";
    ofs << "// This file is part of https://github.com/Cryolite/nyanten.\n";
    ofs << '\n';
    ofs << "#if !defined(NYANTEN_SRC_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_SRC_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD\n";
    ofs << '\n';
    ofs << "#include <nyanten/standard/zipai_table.hpp>\n";
    ofs << "#include <nyanten/standard/core.hpp>\n";
    ofs << '\n';
    ofs << '\n';
    ofs << "namespace Nyanten::Standard_{\n";
    ofs << '\n';
    ofs << "inline constexpr std::array<MapValue, Nyanten::Standard_::zipai_size> zipai_map{";
    for (MapValue const &pack : zipai_map) {
      ofs << pack << "u,";
    }
    ofs << "};\n";
    ofs << '\n';
    ofs << "} // namespace Nyanten::Standard_\n";
    ofs << '\n';
    ofs << "#endif // !defined(NYANTEN_SRC_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD)\n";
  }

  return EXIT_SUCCESS;
}
