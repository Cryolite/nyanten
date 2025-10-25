// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten.

#if !defined(NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD

#include <nyanten/standard/keys.hpp>
#include <nyanten/standard/hash.hpp>
#include <nyanten/standard/core.hpp>
#include <algorithm>
#include <array>
#include <cstdint>


namespace Nyanten::Standard_{

using Nyanten::Standard_::Key0;
using Nyanten::Standard_::Key1;
using Nyanten::Standard_::Key2;
using Nyanten::Standard_::shupai_keys;
using Nyanten::Standard_::zipai_keys;

template<typename ForwardIterator>
std::uint_fast8_t calculateReplacementNumber(ForwardIterator first, std::uint_fast8_t const n)
{
  std::uint_fast8_t const m = n / 3u;

  std::uint_fast32_t const h0 = Nyanten::Standard_::hashShupai(first, first + 9u);
  Key0 const shupai_key0 = shupai_keys[h0];

  std::uint_fast32_t const h1 = Nyanten::Standard_::hashShupai(first + 9u, first + 18u);
  Key0 const shupai_key1 = shupai_keys[h1];
  Key1 const key1 = Nyanten::Standard_::keys1[shupai_key0][shupai_key1];

  std::uint_fast32_t const h2 = Nyanten::Standard_::hashShupai(first + 18u, first + 27u);
  Key0 const shupai_key2 = shupai_keys[h2];
  Key2 const key2 = Nyanten::Standard_::keys2[key1][shupai_key2];

  std::uint_fast32_t const h3 = Nyanten::Standard_::hashZipai(first + 27u, first + 34u);
  Key0 const zipai_key = zipai_keys[h3];
  return Nyanten::Standard_::keys3[key2][zipai_key][m];
}

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_REPLACEMENT_NUMBER_HPP_INCLUDE_GUARD)
