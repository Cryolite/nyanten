// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#include "core.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <array>
#include <stdexcept>
#include <cstdint>
#include <cassert>


namespace {

template<std::size_t N>
using TableImpl = std::array<
  std::array<
    std::uint_fast32_t,
    15u // n = 0, 1, ..., 14
  >,
  N // i = 0, 1, ..., N - 1 (N = 9 or 7)
>;
using ShupaiTableImpl = TableImpl<9u>;
using ZipaiTableImpl = TableImpl<7u>;

template<std::size_t N>
std::uint_fast32_t buildTable(
  std::uint_fast8_t const i, std::uint_fast8_t const n, TableImpl<N> &table)
{
  static_assert(N == 9u || N == 7u);
  assert((i < N));
  assert((n <= 14));

  if (table[i][n] != 0u) {
    return table[i][n];
  }

  for (std::uint_fast8_t c = 0u; c <= 4u; ++c) {
    if (n + c > 14u) {
      break;
    }

    if (i + 1u < N) {
      table[i][n] += buildTable(i + 1u, n + c, table);
    }
    else {
      ++table[i][n];
    }
  }

  return table[i][n];
}

template<std::size_t N>
void dumpTable(TableImpl<N> const &table)
{
  static_assert(N == 9u || N == 7u);

  std::filesystem::path const path(
    N == 9u ? "src/standard/shupai_table.hpp" : "src/standard/zipai_table.hpp");
  std::ofstream ofs(path);
  if (!ofs) {
    throw std::runtime_error("Failed to create the table file.");
  }
  ofs << "// Copyright (c) 2024 Cryolite. All rights reserved.\n";
  ofs << "// SPDX-License-Identifier: MIT\n";
  ofs << "// This file is part of https://github.com/Cryolite/nyanten\n";
  ofs << '\n';
  if (N == 9u) {
    ofs << "#if !defined(NYANTEN_STANDARD_SHUPAI_TABLE_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_STANDARD_SHUPAI_TABLE_HPP_INCLUDE_GUARD\n";
  }
  else {
    ofs << "#if !defined(NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD)\n";
    ofs << "#define NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD\n";
  }
  ofs << '\n';
  ofs << "#include \"core.hpp\"\n";
  ofs << "#include <cstdint>\n";
  ofs << '\n';
  ofs << '\n';
  ofs << "namespace Nyanten::Standard_ {\n";
  ofs << '\n';
  if (N == 9u) {
    ofs << "inline constexpr std::uint_fast32_t shupai_size = ";
  }
  else {
    ofs << "inline constexpr std::uint_fast32_t zipai_size = ";
  }
  ofs << table[0u][0u] << "u;\n";
  ofs << '\n';
  if (N == 9u) {
    ofs << "inline constexpr ShupaiTable shupai_table = {{\n";
  }
  else {
    ofs << "inline constexpr ZipaiTable zipai_table = {{\n";
  }

  for (std::uint_fast8_t i = 0u; i < N; ++i) {
    ofs << "  {{ // i = " << static_cast<unsigned>(i) << '\n';
    for (std::uint_fast8_t n = 0u; n <= 14u; ++n) {
      ofs << "    {{";
      for (std::uint_fast8_t c = 0u; c <= 4u; ++c) {
        std::uint_fast32_t entry = 0u;
        for (std::uint_fast8_t cc = 0u; cc < c; ++cc) {
          if (n + cc < c) {
            break;
          }
          if (n + cc > 14u + c) {
            break;
          }
          if (i + 1u < N) {
            entry += table[i + 1u][n - c + cc];
          }
          else {
            ++entry;
          }
        }
        ofs << entry << ',';
      }
      ofs << "}}, // n = " << static_cast<unsigned>(n) << '\n';
    }
    ofs << "  }},\n";
  }
  ofs << "}};\n";
  ofs << '\n';
  ofs << "} // namespace Nyanten::Standard_\n";
  ofs << '\n';
  if (N == 9u) {
    ofs << "#endif // !defined(NYANTEN_STANDARD_SHUPAI_TABLE_HPP_INCLUDE_GUARD)\n";
  }
  else {
    ofs << "#endif // !defined(NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD)\n";
  }
  ofs << std::flush;
}

} // namespace <anonymous>

int main(int const argc, char const * const * const argv)
{
  if (argc >= 2) {
    throw std::runtime_error("Too many arguments.");
  }

  {
    ShupaiTableImpl shupai_table;
    for (auto &row : shupai_table) {
      row.fill(0u);
    }

    buildTable(0u, 0u, shupai_table);
    assert((shupai_table[0u][0u] == 405350u));

    dumpTable(shupai_table);
  }

  {
    ZipaiTableImpl zipai_table;
    for (auto &row : zipai_table) {
      row.fill(0u);
    }

    buildTable(0u, 0u, zipai_table);
    assert((zipai_table[0u][0u] == 43130));

    dumpTable(zipai_table);
  }
}
