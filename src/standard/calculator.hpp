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
#include <utility>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <cassert>


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
  Calculator() = delete;

  explicit Calculator(std::filesystem::path const &path)
    : p_shupai_map_(std::make_shared<Map>(shupai_size))
    , p_zipai_map_(std::make_shared<Map>(zipai_size))
  {
    using Value = Map::value_type;

    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) {
      throw std::runtime_error("Failed to open the map file.");
    }

    ifs.read(
      reinterpret_cast<char *>(p_shupai_map_->data()), p_shupai_map_->size() * sizeof(Value));
    if (!ifs) {
      throw std::runtime_error("Failed to read the map file.");
    }
    if (ifs.gcount() != static_cast<std::streamsize>(p_shupai_map_->size() * sizeof(Value))) {
      throw std::runtime_error("Failed to read the map file.");
    }

    ifs.read(reinterpret_cast<char *>(p_zipai_map_->data()), p_zipai_map_->size() * sizeof(Value));
    if (!ifs) {
      throw std::runtime_error("Failed to read the map file.");
    }
    if (ifs.gcount() != static_cast<std::streamsize>(p_zipai_map_->size() * sizeof(Value))) {
      throw std::runtime_error("Failed to read the map file.");
    }
  }

  Calculator(Calculator const &) noexcept = default;

  Calculator(Calculator &&rhs) noexcept
    : Calculator(std::as_const(rhs))
  {}

  void swap(Calculator &rhs) noexcept
  {
    p_shupai_map_.swap(rhs.p_shupai_map_);
    p_zipai_map_.swap(rhs.p_zipai_map_);
  }

  friend void swap(Calculator &lhs, Calculator &rhs) noexcept
  {
    lhs.swap(rhs);
  }

  Calculator &operator=(Calculator const &rhs) noexcept
  {
    Calculator(rhs).swap(*this);
    return *this;
  }

  Calculator &operator=(Calculator &&rhs) noexcept
  {
    swap(rhs);
    return *this;
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
  std::uint_fast8_t operator()(
    ForwardIterator first, ForwardIterator last, std::uint_fast8_t const n) const
  {
    assert((!!p_shupai_map_));
    assert((!!p_zipai_map_));

    std::uint_fast8_t const m = n / 3u;

    std::uint_fast32_t const h0 = hashShupai(first, first + 9u);
    std::uint64_t const pack0 = (*p_shupai_map_)[h0];
    Unpack unpack0 = unpackReplacementNumbers(pack0);

    std::uint_fast32_t const h1 = hashShupai(first + 9u, first + 18u);
    std::uint64_t const pack1 = (*p_shupai_map_)[h1];
    Unpack const unpack1 = unpackReplacementNumbers(pack1);
    add_(unpack0, unpack1);

    std::uint_fast32_t const h2 = hashShupai(first + 18u, first + 27u);
    std::uint64_t const pack2 = (*p_shupai_map_)[h2];
    Unpack const unpack2 = unpackReplacementNumbers(pack2);
    add_(unpack0, unpack2);

    std::uint_fast32_t const h3 = hashZipai(first + 27u, first + 34u);
    std::uint64_t const pack3 = (*p_zipai_map_)[h3];
    Unpack const unpack3 = unpackReplacementNumbers(pack3);
    add_(unpack0, unpack3);

    return unpack0[5u + m];
  }

private:
  std::shared_ptr<Map> p_shupai_map_;
  std::shared_ptr<Map> p_zipai_map_;
}; // class Calculator

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_CALCULATOR_HPP_INCLUDE_GUARD)
