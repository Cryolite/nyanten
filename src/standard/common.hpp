// Copyright (c) 2025 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#if !defined(NYANTEN_SRC_STANDARD_COMMON_HPP_INCLUDE_GUARD)
#define NYANTEN_SRC_STANDARD_COMMON_HPP_INCLUDE_GUARD

#include <nyanten/standard/core.hpp>
#include <array>
#include <cstdint>


namespace Nyanten::Standard_{

using Nyanten::Standard_::MapValue;
using Unpack = std::array<std::uint_fast8_t, 9u>;

inline MapValue pack(Unpack unpack)
{
  unpack[5u] -= unpack[0u];
  unpack[6u] -= unpack[1u];
  unpack[7u] -= unpack[2u];
  unpack[8u] -= unpack[3u];
  unpack[3u] -= unpack[2u];
  unpack[2u] -= unpack[1u];
  unpack[1u] -= unpack[0u];

  MapValue pack = 0u;
  pack += unpack[0u] * (4u * 4u * 4u * 3u * 3u * 3u * 3u * 3u);
  pack += unpack[1u] * (4u * 4u * 3u * 3u * 3u * 3u * 3u);
  pack += unpack[2u] * (4u * 3u * 3u * 3u * 3u * 3u);
  pack += unpack[3u] * (3u * 3u * 3u * 3u * 3u);
  pack += unpack[4u] * (3u * 3u * 3u * 3u);
  pack += unpack[5u] * (3u * 3u * 3u);
  pack += unpack[6u] * (3u * 3u);
  pack += unpack[7u] * (3u);
  pack += unpack[8u];

  return pack;
}

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_SRC_STANDARD_COMMON_HPP_INCLUDE_GUARD)
