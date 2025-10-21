// Copyright (c) 2025 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#include "zipai_map.hpp"
#include "shupai_map.hpp"
#include "common.hpp"
#include <nyanten/standard/zipai_table.hpp>
#include <nyanten/standard/shupai_table.hpp>
#include <nyanten/standard/core.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <vector>
#include <array>
#include <cstdint>
#include <cstdlib>


namespace {

using Nyanten::Standard_::shupai_size;
using Nyanten::Standard_::zipai_size;
using Nyanten::Standard_::MapValue;
using Nyanten::Standard_::Key0;
using KeyMap0 = std::unordered_map<MapValue, Key0>;
using MapValues = std::vector<MapValue>;
using Nyanten::Standard_::Unpack;
using Nyanten::Standard_::pack;
using Nyanten::Standard_::Key1;
using KeyMap1 = std::unordered_map<MapValue, Key1>;
using Nyanten::Standard_::Key2;
using KeyMap2 = std::unordered_map<MapValue, Key2>;
using Key3 = std::uint_fast8_t;
using KeyMap3 = std::unordered_map<MapValue, Key3>;

inline Unpack unpack(MapValue pack)
{
  std::array<std::uint_fast8_t, 9u> result;

  result[0u] = pack / (4u * 4u * 4u * 3u * 3u * 3u * 3u * 3u);
  pack %= 4u * 4u * 4u * 3u * 3u * 3u * 3u * 3u;
  result[1u] = pack / (4u * 4u * 3u * 3u * 3u * 3u * 3u);
  pack %= 4u * 4u * 3u * 3u * 3u * 3u * 3u;
  result[2u] = pack / (4u * 3u * 3u * 3u * 3u * 3u);
  pack %= 4u * 3u * 3u * 3u * 3u * 3u;
  result[3u] = pack / (3u * 3u * 3u * 3u * 3u);
  pack %= 3u * 3u * 3u * 3u * 3u;
  result[4u] = pack / (3u * 3u * 3u * 3u);
  pack %= 3u * 3u * 3u * 3u;
  result[5u] = pack / (3u * 3u * 3u);
  pack %= 3u * 3u * 3u;
  result[6u] = pack / (3u * 3u);
  pack %= 3u * 3u;
  result[7u] = pack / (3u);
  pack %= 3u;
  result[8u] = pack;

  result[1u] += result[0u];
  result[2u] += result[1u];
  result[3u] += result[2u];
  result[5u] += result[0u];
  result[6u] += result[1u];
  result[7u] += result[2u];
  result[8u] += result[3u];

  return result;
}

inline void add(Unpack &lhs, Unpack const &rhs)
{
  for (std::uint_fast8_t i = 9u; i > 4u;) {
    --i;
    std::uint_fast8_t r = std::min(lhs[i], rhs[i]);
    for (std::uint_fast8_t j = 0u; j < i - 4u; ++j) {
      r = std::min({r, static_cast<std::uint_fast8_t>(lhs[i - j - 1u] + rhs[j]), static_cast<std::uint_fast8_t>(lhs[j] + rhs[i - j - 1u])});
    }
    lhs[i] = r;
  }

  for (std::uint_fast8_t i = 4u; i > 0u;) {
    --i;
    std::uint_fast8_t r = std::min(lhs[i], rhs[i]);
    for (std::uint_fast8_t j = 0u; j < i; ++j) {
      r = std::min({r, static_cast<std::uint_fast8_t>(lhs[i - j - 1u] + rhs[j]), static_cast<std::uint_fast8_t>(lhs[j] + rhs[i - j - 1u])});
    }
    lhs[i] = r;
  }
}

std::pair<KeyMap0, KeyMap0> buildStage0(std::ofstream &ofs)
{
  KeyMap0 shupai_keys = [&]() {
    KeyMap0 result;
    std::set<MapValue> shupai_map_values;
    for (MapValue const v : Nyanten::Standard_::shupai_map) {
      shupai_map_values.insert(v);
    }
    Key0 k = 0u;
    for (auto const v : shupai_map_values) {
      result.emplace(v, k++);
    }
    return result;
  }();

  ofs << "inline constexpr std::array<Key0, Nyanten::Standard_::shupai_size> shupai_keys{";
  for (MapValue const v : Nyanten::Standard_::shupai_map) {
    ofs << static_cast<std::uint32_t>(shupai_keys.at(v)) << "u,";
  }
  ofs << "};\n";
  ofs << '\n';

  KeyMap0 zipai_keys = [&]() {
    KeyMap0 result;
    std::set<MapValue> zipai_map_values;
    for (MapValue const v : Nyanten::Standard_::zipai_map) {
      zipai_map_values.insert(v);
    }
    Key0 k = 0u;
    for (auto const v : zipai_map_values) {
      result.emplace(v, k++);
    }
    return result;
  }();

  ofs << "inline constexpr std::array<Key0, Nyanten::Standard_::zipai_size> zipai_keys{";
  for (MapValue const v : Nyanten::Standard_::zipai_map) {
    ofs << static_cast<std::uint32_t>(zipai_keys.at(v)) << "u,";
  }
  ofs << "};\n";
  ofs << '\n';

  return {std::move(shupai_keys), std::move(zipai_keys)};
}

std::pair<KeyMap1, MapValues> buildStage1(KeyMap0 const &shupai_keys, MapValues const &shupai_map_values, std::ofstream &ofs)
{
  KeyMap1 keys1 = [&]() {
    KeyMap1 result;
    std::set<MapValue> map_values1;
    for (std::uint_fast8_t i = 0u; i < shupai_keys.size(); ++i) {
      MapValue const v0 = shupai_map_values[i];
      Unpack const unpack0 = unpack(v0);
      for (std::uint_fast8_t j = 0u; j < shupai_keys.size(); ++j) {
        MapValue const v1 = shupai_map_values[j];
        Unpack const unpack1 = unpack(v1);
        Unpack unpack2 = unpack0;
        add(unpack2, unpack1);
        MapValue const v2 = pack(unpack2);
        map_values1.insert(v2);
      }
    }
    Key1 k = 0u;
    for (auto const v : map_values1) {
      result.emplace(v, k++);
    }
    return result;
  }();

  MapValues map_values1(keys1.size());
  for (auto const [v, k] : keys1) {
    map_values1[k] = v;
  }

  ofs << "inline constexpr std::array<std::array<Key1, " << shupai_keys.size() << "u>, " << shupai_keys.size() << "u> keys1{{\n";

  for (std::uint_fast8_t i = 0u; i < shupai_keys.size(); ++i) {
    ofs << "  {{";
    MapValue const v0 = shupai_map_values[i];
    Unpack const unpack0 = unpack(v0);
    for (std::uint_fast8_t j = 0u; j < shupai_keys.size(); ++j) {
      MapValue const v1 = shupai_map_values[j];
      Unpack const unpack1 = unpack(v1);
      Unpack unpack2 = unpack0;
      add(unpack2, unpack1);
      MapValue const v2 = pack(unpack2);
      ofs << static_cast<unsigned>(keys1.at(v2)) << "u,";
    }
    ofs << "}},\n";
  }

  ofs << "}};\n";
  ofs << '\n';

  return {std::move(keys1), std::move(map_values1)};
}

std::pair<KeyMap2, MapValues> buildStage2(
  KeyMap1 const &keys1,
  MapValues const &map_values1,
  KeyMap0 const &shupai_keys,
  MapValues const &shupai_map_values,
  std::ofstream &ofs)
{
  KeyMap2 keys2 = [&]() {
    KeyMap2 result;
    std::set<MapValue> map_values2;
    for (std::uint_fast8_t i = 0u; i < keys1.size(); ++i) {
      MapValue const v0 = map_values1[i];
      Unpack const unpack0 = unpack(v0);
      for (std::uint_fast8_t j = 0u; j < shupai_keys.size(); ++j) {
        MapValue const v1 = shupai_map_values[j];
        Unpack const unpack1 = unpack(v1);
        Unpack unpack2 = unpack0;
        add(unpack2, unpack1);
        MapValue const v2 = pack(unpack2);
        map_values2.insert(v2);
      }
    }
    Key2 k = 0u;
    for (auto const v : map_values2) {
      result.emplace(v, k++);
    }
    return result;
  }();

  MapValues map_values2(keys2.size());
  for (auto const [v, k] : keys2) {
    map_values2[k] = v;
  }

  ofs << "inline constexpr std::array<std::array<Key2, " << shupai_keys.size() << "u>, " << keys1.size() << "u> keys2{{\n";

  for (std::uint_fast8_t i = 0u; i < keys1.size(); ++i) {
    ofs << "  {{";
    MapValue const v0 = map_values1[i];
    Unpack const unpack0 = unpack(v0);
    for (std::uint_fast8_t j = 0u; j < shupai_keys.size(); ++j) {
      MapValue const v1 = shupai_map_values[j];
      Unpack const unpack1 = unpack(v1);
      Unpack unpack2 = unpack0;
      add(unpack2, unpack1);
      MapValue const v2 = pack(unpack2);
      ofs << static_cast<unsigned>(keys2.at(v2)) << "u,";
    }
    ofs << "}},\n";
  }

  ofs << "}};\n";
  ofs << '\n';

  return {std::move(keys2), std::move(map_values2)};
}

std::pair<KeyMap3, MapValues> buildStage3(
  KeyMap2 const &keys2,
  MapValues const &map_values2,
  KeyMap0 const &zipai_keys,
  MapValues const &zipai_map_values,
  std::ofstream &ofs)
{
  KeyMap3 keys3 = [&]() {
    KeyMap3 result;
    std::set<MapValue> map_values3;
    for (std::uint_fast8_t i = 0u; i < keys2.size(); ++i) {
      MapValue const v0 = map_values2[i];
      Unpack const unpack0 = unpack(v0);
      for (std::uint_fast8_t j = 0u; j < zipai_keys.size(); ++j) {
        MapValue const v1 = zipai_map_values[j];
        Unpack const unpack1 = unpack(v1);
        Unpack unpack2 = unpack0;
        add(unpack2, unpack1);
        MapValue const v2 = pack(unpack2);
        map_values3.insert(v2);
      }
    }
    Key3 k = 0u;
    for (auto const v : map_values3) {
      result.emplace(v, k++);
    }
    return result;
  }();

  MapValues map_values3(keys3.size());
  for (auto const [v, k] : keys3) {
    map_values3[k] = v;
  }

  ofs << "inline constexpr std::array<std::array<std::array<std::uint_fast8_t, 5u>, " << zipai_keys.size() << "u>, " << keys2.size() << "u> keys3{{\n";

  for (std::uint_fast8_t i = 0u; i < keys2.size(); ++i) {
    ofs << "  {{\n";
    MapValue const v0 = map_values2[i];
    Unpack const unpack0 = unpack(v0);
    for (std::uint_fast8_t j = 0u; j < zipai_keys.size(); ++j) {
      MapValue const v1 = zipai_map_values[j];
      Unpack const unpack1 = unpack(v1);
      Unpack unpack2 = unpack0;
      add(unpack2, unpack1);
      MapValue const v2 = pack(unpack2);
      ofs << "    {{";
      for (std::uint_fast8_t m = 0u; m <= 4u; ++m) {
        ofs << static_cast<unsigned>(unpack2[4u + m]) << "u,";
      }
      ofs << "}},\n";
    }
    ofs << "  }},\n";
  }

  ofs << "}};\n";
  ofs << '\n';

  return {std::move(keys3), std::move(map_values3)};
}

} // namespace *unnamed*

int main(int const argc, char const * const * const argv)
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <PATH TO KEYS FILE>" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const keys_path(argv[1]);

  std::ofstream ofs(keys_path);
  if (!ofs) {
    std::cerr << "Failed to create the keys file." << std::endl;
    return EXIT_FAILURE;
  }

  ofs << "// Copyright (c) 2025 Cryolite. All rights reserved.\n";
  ofs << "// SPDX-License-Identifier: MIT\n";
  ofs << "// This file is part of https://github.com/Cryolite/Nyanten.\n";
  ofs << '\n';
  ofs << "#if !defined(NYANTEN_STANDARD_KEYS_HPP_INCLUDE_GUARD)\n";
  ofs << "#define NYANTEN_STANDARD_KEYS_HPP_INCLUDE_GUARD\n";
  ofs << '\n';
  ofs << "#include <nyanten/standard/zipai_table.hpp>\n";
  ofs << "#include <nyanten/standard/shupai_table.hpp>\n";
  ofs << "#include <nyanten/standard/core.hpp>\n";
  ofs << "#include <array>\n";
  ofs << "#include <cstdint>\n";
  ofs << '\n';
  ofs << '\n';
  ofs << "namespace Nyanten::Standard_{\n";
  ofs << '\n';

  auto [shupai_keys, zipai_keys] = buildStage0(ofs);
  std::cout << "Shupai Key0 Size: " << shupai_keys.size() << std::endl;
  std::cout << "Zipai Key0 Size: " << zipai_keys.size() << std::endl;

  MapValues shupai_map_values(shupai_keys.size());
  for (auto const [v, k] : shupai_keys) {
    shupai_map_values[k] = v;
  }

  MapValues zipai_map_values(zipai_keys.size());
  for (auto const [v, k] : zipai_keys) {
    zipai_map_values[k] = v;
  }

  auto [keys1, map_values1] = buildStage1(shupai_keys, shupai_map_values, ofs);
  std::cout << "Key1 Size: " << static_cast<unsigned>(keys1.size()) << std::endl;

  auto [keys2, map_values2] = buildStage2(keys1, map_values1, shupai_keys, shupai_map_values, ofs);
  std::cout << "Key2 Size: " << static_cast<unsigned>(keys2.size()) << std::endl;

  auto [keys3, map_values3] = buildStage3(keys2, map_values2, zipai_keys, zipai_map_values, ofs);
  std::cout << "Key3 Size: " << static_cast<unsigned>(keys3.size()) << std::endl;

  ofs << "} // namespace Nyanten::Standard_\n";
  ofs << '\n';
  ofs << "#endif // !defined(NYANTEN_STANDARD_KEYS_HPP_INCLUDE_GUARD)\n";
  ofs << std::flush;

  return EXIT_SUCCESS;
}
