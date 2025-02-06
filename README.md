# Nyanten

Nyanten is a C++ library for fast calculation of deficiency number (a.k.a. shanten number, 向聴数).

# Installation

```bash
cmake -S /PATH/TO/nyanten -B /PATH/TO/nyanten/build && cmake --install /PATH/TO/nyanten/build --prefix /PATH/TO/INSTALL_PREFIX
```

After the installation, please add `/PATH/TO/INSTALL_PREFIX/include` to your include path. Since Nyanten is a header-only library, you can also use it by adding the root directory of the Nyanten repository to your include path, even if you do not perform the installation steps described above.

# Typical Usage

```cpp
#include <nyanten/replacement_number.hpp>

int main()
{
  std::vector<unsigned> hand;
  // Set the count of each tile to `hand`.
  unsigned const replacement_number = Nyanten::calculateReplacementNumber(hand);
}
```

# Interfaces

### `template<typename ForwardIterator>`<br/>`std::uint_fast8_t Nyanten::calculateReplacementNumber(ForwardIterator first, ForwardIterator last)`

#### Preconditions

- `std::distance(first, last) == 34`
- For every `iter` in the range `[first, last)`, `0 <= *iter` and `*iter <= 4`
- The sum of the numbers in the range `[first, last)` is less or equal to 14.
- The sum of the numbers in the range `[first, last)` is congruent to 1 or 2 modulo 3.

#### Effects

Calculate the replacement number, which is equal to the deficiency number (a.k.a. Shanten number, 向聴数) plus one. The hand is represented by the range `[first, last)`. Each element of the range corresponds to the count of each tile type. If there are melds (副露), the count must exclude the melded tiles. The correspondence between the element indices of the range and the tile types is as shown in the table below.

| Index |      0 |      1 |      2 |      3 |      4 |      5 |      6 |      7 |      8 |
|-------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| Tile  | 🀇 (1m) | 🀈 (2m) | 🀉 (3m) | 🀊 (4m) | 🀋 (5m) | 🀌 (6m) | 🀍 (7m) | 🀎 (8m) | 🀏 (9m) |

| Index |      9 |     10 |     11 |     12 |     13 |     14 |     15 |     16 |     17 |
|-------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| Tile  | 🀙 (1p) | 🀚 (2p) | 🀛 (3p) | 🀜 (4p) | 🀝 (5p) | 🀞 (6p) | 🀟 (7p) | 🀠 (8p) | 🀡 (9p) |

| Index |     18 |     19 |     20 |     21 |     22 |     23 |     24 |     25 |     26 |
|-------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| Tile  | 🀐 (1s) | 🀑 (2s) | 🀒 (3s) | 🀓 (4s) | 🀔 (5s) | 🀕 (6s) | 🀖 (7s) | 🀗 (8s) | 🀘 (9s) |

| Index |    27 |    28 |    29 |    30 |        31 |        32 |      33 |
|-------|-------|-------|-------|-------|-----------|-----------|---------|
| Tile  | 🀀 (E) | 🀁 (S) | 🀂 (W) | 🀃 (N) | 🀆 (White) | 🀅 (Green) | 🀄 (Red) |

### `template<typename ForwardRange>`<br/>`std::uint_fast8_t Nyanten::calculateReplacementNumber(ForwardRange const &r)`

Call `Nyanten::calculateReplacementNumber(std::cbegin(r), std::cend(r))`.
