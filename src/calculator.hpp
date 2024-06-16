#if !defined(NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD)
#define NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD

#include "thirteen_orphans/replacement_number.hpp"
#include "seven_pairs/replacement_number.hpp"
#include "standard/calculator.hpp"
#include <filesystem>
#include <algorithm>
#include <cstdint>


namespace Nyanten {

class Calculator
{
public:
  Calculator() = delete;

  explicit Calculator(std::filesystem::path const & path)
    : standard_calculator_(path)
  {}

  Calculator(Calculator const &) noexcept = default;

  Calculator(Calculator &&) noexcept = default;

  void swap(Calculator &rhs) noexcept
  {
    standard_calculator_.swap(rhs.standard_calculator_);
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

  template<typename ForwardIterator>
  std::uint_fast8_t operator()(ForwardIterator first, ForwardIterator last) const
  {
    std::uint_fast8_t const n = [&]() {
      std::uint_fast8_t i = 0u;
      std::uint_fast8_t n = 0u;
      for (ForwardIterator iter = first; iter != last; ++iter) {
        if (*iter < 0) {
          throw std::invalid_argument("An invalid hand.");
        }
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

    std::uint_fast8_t const r0 = standard_calculator_(first, last, n);
    std::uint_fast8_t const r1 = SevenPairs_::calculateReplacementNumber(first, last, n);
    std::uint_fast8_t const r2 = ThirteenOrphans_::calculateReplacementNumber(first, last, n);
    return std::min({r0, r1, r2});
  }

private:
  Standard_::Calculator standard_calculator_;
}; // class Calculator

} // namespace Nyanten

#endif // !defined(NYANTEN_CALCULATOR_HPP_INCLUDE_GUARD)
