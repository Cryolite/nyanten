// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#include <nyanten/standard/hash.hpp>
#include <nyanten/standard/zipai_table.hpp>
#include <nyanten/standard/shupai_table.hpp>
#include <nyanten/standard/core.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
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
MapValue packReplacementNumbers(std::array<std::uint_fast8_t, N> const &hand)
{
  static_assert(N == 7u || N == 9u);

  MapValue pack{};
  for (std::uint_fast8_t h = 0u; h <= 1u; ++h) {
    for (std::uint_fast8_t m = 0u; m <= 4u; ++m) {
      std::uint_fast8_t replacement_number = 7u;
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
      pack[m] |= static_cast<std::uint8_t>(replacement_number << (h * 4u));
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
    ofs << "// This file is part of https://github.com/Cryolite/nyanten\n";
    ofs << '\n';
    ofs << "#if !defined(NYANTEN_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD\n";
    ofs << '\n';
    ofs << "#include <nyanten/standard/shupai_table.hpp>\n";
    ofs << "#include <nyanten/standard/core.hpp>\n";
    ofs << "#include <cstdint>\n";
    ofs << '\n';
    ofs << '\n';
    ofs << "namespace Nyanten::Standard_{\n";
    ofs << '\n';
    ofs << "inline constexpr std::array<MapValue, Nyanten::Standard_::shupai_size> shupai_map{{\n";
    for (MapValue const &entry : shupai_map) {
      ofs << "  {{";
      for (std::uint8_t const pack : entry) {
        ofs << static_cast<unsigned>(pack) << "u,";
      }
      ofs << "}},\n";
    }
    ofs << "}};\n";
    ofs << '\n';
    ofs << "} // namespace Nyanten::Standard_\n";
    ofs << '\n';
    ofs << "#endif // !defined(NYANTEN_STANDARD_SHUPAI_MAP_HPP_INCLUDE_GUARD)\n";
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
    ofs << "// This file is part of https://github.com/Cryolite/nyanten\n";
    ofs << '\n';
    ofs << "#if !defined(NYANTEN_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD\n";
    ofs << '\n';
    ofs << "#include <nyanten/standard/zipai_table.hpp>\n";
    ofs << "#include <nyanten/standard/core.hpp>\n";
    ofs << "#include <cstdint>\n";
    ofs << '\n';
    ofs << '\n';
    ofs << "namespace Nyanten::Standard_{\n";
    ofs << '\n';
    ofs << "inline constexpr std::array<MapValue, Nyanten::Standard_::zipai_size> zipai_map{{\n";
    for (MapValue const &entry : zipai_map) {
      ofs << "  {{";
      for (std::uint8_t const pack : entry) {
        ofs << static_cast<unsigned>(pack) << "u,";
      }
      ofs << "}},\n";
    }
    ofs << "}};\n";
    ofs << '\n';
    ofs << "} // namespace Nyanten::Standard_\n";
    ofs << '\n';
    ofs << "#endif // !defined(NYANTEN_STANDARD_ZIPAI_MAP_HPP_INCLUDE_GUARD)\n";
  }

  return EXIT_SUCCESS;
}
