#if !defined(NYANTEN_STANDARD_CORE_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_CORE_HPP_INCLUDE_GUARD

#include <vector>
#include <array>
#include <cstdint>


namespace Nyanten::Standard_ {

using ShupaiTable = std::array<
  std::array<
    std::array<
      std::uint_fast32_t,
      5u // s = 0, 1, 2, 3, 4
    >,
    15u // n = 0, 1, ..., 14
  >,
  9u // i = 0, 1, ..., 8
>;
using ZipaiTable = std::array<
  std::array<
    std::array<
      std::uint_fast32_t,
      5u // s = 0, 1, 2, 3, 4
    >,
    15u // n = 0, 1, ..., 14
  >,
  7u // i = 0, 1, ..., 6
>;

using Map = std::vector<std::uint64_t>;

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_CORE_HPP_INCLUDE_GUARD)
