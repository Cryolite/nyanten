# Nyanten

Nyanten is a C++ library for fast calculation of deficiency number (a.k.a. shanten number, 向聴数).

# Typical Usage

```cpp
#include <nyanten/calculator.hpp>

int main()
{
  std::vector<unsigned> hand;
  // Set the count of each tile to `hand`.
  Nyanten::Calculator calculator("PATH/TO/map.bin");
  unsigned const deficiency_number = calculator(hand);
}
```

# Interfaces

### `explicit Nyanten::Calculator::Calculator(std::filesystem::path const &path)`

Construct an instance of the class `Nyanten::Calculator`. The argument `path` must be a path to the `map.bin` file.

### `Nyanten::Calculator::Calculator(Calculator const &) noexcept`

The copy constructor.

### `Nyanten::Calculator::Calculator(Calculator &&) noexcept`

The move constructor.

### `void Nyanten::Calculator::swap(Calculator &rhs) noexcept`

Swap the object states between `*this` and `rhs`.

### `void Nyanten::swap(Nyanten::Calculator &lhs, Nyanten::Calculator &rhs) noexcept`

Call `lhs.swap(rhs)`.

### `Nyanten::Calculator &operator=(Nyanten::Calculator const &) noexcept`

The copy assignment operator.

### `Nyanten::Calculator &operator=(Nyanten::Calculator &&) noexcept`

The move assignment operator.

### `template<typename ForwardIterator>`<br/>`std::uint_fast8_t Nyanten::Calculator::operator()(ForwardIterator first, ForwardIterator last) noexcept`

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

### `template<typename ForwardRange>`<br/>`std::uint_fast8_t Nyanten::Calculator::operator()(ForwardRange const &r) const`

Call `(*this)(std::cbegin(r), std::cend(r))`.
