// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#if !defined(NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD

#include <nyanten/standard/zipai_map.hpp>
#include <nyanten/standard/shupai_map.hpp>
#include <nyanten/standard/hash.hpp>
#include <nyanten/standard/core.hpp>
#include <algorithm>
#include <array>
#include <cstdint>


namespace Nyanten::Standard_{

using Nyanten::Standard_::shupai_map;
using Nyanten::Standard_::zipai_map;
using Nyanten::Standard_::MapValue;
using Unpack = std::array<std::uint_fast8_t, 10u>;

inline Unpack unpack(MapValue const &pack)
{
  return {
    0u,
    static_cast<uint_fast8_t>((pack >> (3u * 0u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 1u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 2u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 3u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 4u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 5u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 6u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 7u)) & 0x07u),
    static_cast<uint_fast8_t>((pack >> (3u * 8u)) & 0x07u),
  };
}

inline void add(Unpack &lhs, Unpack const &rhs)
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

template<typename ForwardIterator>
std::uint_fast8_t calculateReplacementNumber(
  ForwardIterator first, ForwardIterator last, std::uint_fast8_t const n)
{
  std::uint_fast8_t const m = n / 3u;

  std::uint_fast32_t const h0 = Nyanten::Standard_::hashShupai(first, first + 9u);
  MapValue const &pack0 = shupai_map[h0];
  Unpack unpack0 = unpack(pack0);

  std::uint_fast32_t const h1 = Nyanten::Standard_::hashShupai(first + 9u, first + 18u);
  MapValue const &pack1 = shupai_map[h1];
  Unpack const unpack1 = unpack(pack1);
  Nyanten::Standard_::add(unpack0, unpack1);

  std::uint_fast32_t const h2 = Nyanten::Standard_::hashShupai(first + 18u, first + 27u);
  MapValue const &pack2 = shupai_map[h2];
  Unpack const unpack2 = unpack(pack2);
  Nyanten::Standard_::add(unpack0, unpack2);

  std::uint_fast32_t const h3 = Nyanten::Standard_::hashZipai(first + 27u, first + 34u);
  MapValue const &pack3 = zipai_map[h3];
  Unpack const unpack3 = unpack(pack3);
  Nyanten::Standard_::add(unpack0, unpack3);

  return unpack0[5u + m];
}

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
