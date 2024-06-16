#if !defined(NYANTEN_THIRTEEN_ORPHANS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
#define NYANTEN_THIRTEEN_ORPHANS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD

#include <iterator>
#include <stdexcept>
#include <limits>
#include <cstdint>


namespace Nyanten::ThirteenOrphans_ {

template<typename ForwardIterator>
std::uint_fast8_t calculateReplacementNumber(
  ForwardIterator first, ForwardIterator last, std::uint_fast8_t const n)
{
  if (n != 13u && n != 14) {
    return std::numeric_limits<std::uint_fast8_t>::max();
  }

  std::uint_fast8_t num_kinds = 0u;
  bool head = false;
  if (*first >= 1) {
    // Has 1m.
    ++num_kinds;
    head = head || *first >= 2;
  }
  std::advance(first, 8);
  if (*first >= 1) {
    // Has 9m.
    ++num_kinds;
    head = head || *first >= 2;
  }
  ++first;
  if (*first >= 1) {
    // Has 1p.
    ++num_kinds;
    head = head || *first >= 2;
  }
  std::advance(first, 8);
  if (*first >= 1) {
    // Has 9p.
    ++num_kinds;
    head = head || *first >= 2;
  }
  ++first;
  if (*first >= 1) {
    // Has 1s.
    ++num_kinds;
    head = head || *first >= 2;
  }
  std::advance(first, 8);
  if (*first >= 1) {
    // Has 9s.
    ++num_kinds;
    head = head || *first >= 2;
  }
  ++first;
  for (; first != last; ++first) {
    if (*first >= 1) {
      ++num_kinds;
      head = head || *first >= 2;
    }
  }

  return 14u - num_kinds - (head ? 1u : 0u);
}

} // namespace Nyanten::ThirteenOrphans_

#endif // !defined(NYANTEN_THIRTEEN_ORPHANS_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
