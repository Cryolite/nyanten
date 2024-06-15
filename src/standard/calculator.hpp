#if !defined(NYANTEN_STANDARD_CALCULATOR_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_CALCULATOR_HPP_INCLUDE_GUARD

#include "hash.hpp"
#include "zipai_table.hpp"
#include "shupai_table.hpp"
#include "core.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <algorithm>
#include <array>
#include <stdexcept>
#include <cstdint>


namespace Nyanten::Standard_ {

using Nyanten::Standard_::shupai_size;
using Nyanten::Standard_::zipai_size;

using Unpack = std::array<std::uint_fast8_t, 10u>;

Unpack unpackReplacementNumbers(std::uint64_t pack)
{
  Unpack unpack{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
  for (std::uint_fast8_t h = 0u; h <= 1u; ++h) {
    for (std::uint_fast8_t m = 0u; m <= 4u; ++m) {
      unpack[h * 5u + m] = pack % 10u;
      pack /= 10u;
    }
  }
  return unpack;
}

class Calculator
{
public:
  explicit Calculator(std::filesystem::path const &path)
    : shupai_map_(shupai_size)
    , zipai_map_(zipai_size)
  {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) {
      throw std::runtime_error("Failed to open the map file.");
    }

    ifs.read(
      reinterpret_cast<char *>(shupai_map_.data()), shupai_map_.size() * sizeof(std::uint64_t));
    if (!ifs) {
      throw std::runtime_error("Failed to read the map file.");
    }
    if (ifs.gcount() != static_cast<long>(shupai_map_.size() * sizeof(std::uint64_t))) {
      throw std::runtime_error("Failed to read the map file.");
    }

    ifs.read(
      reinterpret_cast<char *>(zipai_map_.data()), zipai_map_.size() * sizeof(std::uint64_t));
    if (!ifs) {
      throw std::runtime_error("Failed to read the map file.");
    }
    if (ifs.gcount() != static_cast<long>(zipai_map_.size() * sizeof(std::uint64_t))) {
      throw std::runtime_error("Failed to read the map file.");
    }
  }

private:
  static void add_(Unpack &lhs, Unpack const &rhs)
  {
    for (std::uint_fast8_t i = 10u; i > 5u;) {
      --i;
      std::uint_fast8_t r = std::min<std::uint_fast8_t>(lhs[i] + rhs[0u], lhs[0u] + rhs[i]);
      for (std::uint_fast8_t j = 5u; j < i; ++j) {
        r = std::min({
          r,
          static_cast<std::uint_fast8_t>(lhs[j] + rhs[i - j]),
          static_cast<std::uint_fast8_t>(lhs[i - j] + rhs[j])});
      }
      lhs[i] = r;
    }

    for (std::uint_fast8_t i = 5u; i > 0u;) {
      --i;
      std::uint_fast8_t r = lhs[i] + rhs[0u];
      for (std::uint_fast8_t j = 0u; j < i; ++j) {
        r = std::min<std::uint_fast8_t>(r, lhs[j] + rhs[i - j]);
      }
      lhs[i] = r;
    }
  }

public:
  template<typename ForwardIterator>
  std::uint_fast8_t operator()(ForwardIterator first, ForwardIterator last) const
  {
    std::uint_fast8_t const n = [&]() {
      std::uint_fast8_t i = 0u;
      std::uint_fast8_t n = 0u;
      for (ForwardIterator iter = first; iter != last; ++iter) {
        if (*iter > 4) {
          throw std::invalid_argument("An invalid hand.");
        }
        ++i;
        n += *iter;
      }
      if (i != 34u) {
        throw std::invalid_argument("An invalid hand.");
      }
      if (n > 14u) {
        throw std::invalid_argument("An invalid hand.");
      }
      if (n % 3u == 0u) {
        throw std::invalid_argument("An invalid hand.");
      }
      return n;
    }();
    std::uint_fast8_t const m = n / 3u;

    std::uint_fast32_t const h0 = hashShupai(first, first + 9u);
    std::uint64_t const pack0 = shupai_map_[h0];
    Unpack unpack0 = unpackReplacementNumbers(pack0);

    std::uint_fast32_t const h1 = hashShupai(first + 9u, first + 18u);
    std::uint64_t const pack1 = shupai_map_[h1];
    Unpack const unpack1 = unpackReplacementNumbers(pack1);
    add_(unpack0, unpack1);

    std::uint_fast32_t const h2 = hashShupai(first + 18u, first + 27u);
    std::uint64_t const pack2 = shupai_map_[h2];
    Unpack const unpack2 = unpackReplacementNumbers(pack2);
    add_(unpack0, unpack2);

    std::uint_fast32_t const h3 = hashZipai(first + 27u, first + 34u);
    std::uint64_t const pack3 = zipai_map_[h3];
    Unpack const unpack3 = unpackReplacementNumbers(pack3);
    add_(unpack0, unpack3);

    return unpack0[5u + m];
  }

private:
  Map shupai_map_;
  Map zipai_map_;
}; // class Calculator

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_CALCULATOR_HPP_INCLUDE_GUARD)
