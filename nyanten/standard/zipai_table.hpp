// Copyright (c) 2024 Cryolite. All rights reserved.
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/nyanten

#if !defined(NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD)
#define NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD

#include <nyanten/standard/core.hpp>
#include <cstdint>


namespace Nyanten::Standard_{

inline constexpr std::uint_fast32_t zipai_size = 43130u;

inline constexpr ZipaiTable zipai_table = {{
  {{ // i = 0
    {{0,0,0,0,0,}}, // n = 0
    {{0,11880,0,0,0,}}, // n = 1
    {{0,10374,22254,0,0,}}, // n = 2
    {{0,8688,19062,30942,0,}}, // n = 3
    {{0,6937,15625,25999,37879,}}, // n = 4
    {{0,5251,12188,20876,31250,}}, // n = 5
    {{0,0,5251,12188,20876,}}, // n = 6
    {{0,0,0,5251,12188,}}, // n = 7
    {{0,0,0,0,5251,}}, // n = 8
    {{0,0,0,0,0,}}, // n = 9
    {{0,0,0,0,0,}}, // n = 10
    {{0,0,0,0,0,}}, // n = 11
    {{0,0,0,0,0,}}, // n = 12
    {{0,0,0,0,0,}}, // n = 13
    {{0,0,0,0,0,}}, // n = 14
  }},
  {{ // i = 1
    {{0,0,0,0,0,}}, // n = 0
    {{0,2878,0,0,0,}}, // n = 1
    {{0,2693,5571,0,0,}}, // n = 2
    {{0,2438,5131,8009,0,}}, // n = 3
    {{0,2118,4556,7249,10127,}}, // n = 4
    {{0,1753,3871,6309,9002,}}, // n = 5
    {{0,1372,3125,5243,7681,}}, // n = 6
    {{0,1007,2379,4132,6250,}}, // n = 7
    {{0,687,1694,3066,4819,}}, // n = 8
    {{0,432,1119,2126,3498,}}, // n = 9
    {{0,0,432,1119,2126,}}, // n = 10
    {{0,0,0,432,1119,}}, // n = 11
    {{0,0,0,0,432,}}, // n = 12
    {{0,0,0,0,0,}}, // n = 13
    {{0,0,0,0,0,}}, // n = 14
  }},
  {{ // i = 2
    {{0,0,0,0,0,}}, // n = 0
    {{0,620,0,0,0,}}, // n = 1
    {{0,610,1230,0,0,}}, // n = 2
    {{0,590,1200,1820,0,}}, // n = 3
    {{0,555,1145,1755,2375,}}, // n = 4
    {{0,503,1058,1648,2258,}}, // n = 5
    {{0,435,938,1493,2083,}}, // n = 6
    {{0,355,790,1293,1848,}}, // n = 7
    {{0,270,625,1060,1563,}}, // n = 8
    {{0,190,460,815,1250,}}, // n = 9
    {{0,122,312,582,937,}}, // n = 10
    {{0,70,192,382,652,}}, // n = 11
    {{0,35,105,227,417,}}, // n = 12
    {{0,15,50,120,242,}}, // n = 13
    {{0,0,15,50,120,}}, // n = 14
  }},
  {{ // i = 3
    {{0,0,0,0,0,}}, // n = 0
    {{0,125,0,0,0,}}, // n = 1
    {{0,125,250,0,0,}}, // n = 2
    {{0,125,250,375,0,}}, // n = 3
    {{0,124,249,374,499,}}, // n = 4
    {{0,121,245,370,495,}}, // n = 5
    {{0,115,236,360,485,}}, // n = 6
    {{0,105,220,341,465,}}, // n = 7
    {{0,90,195,310,431,}}, // n = 8
    {{0,72,162,267,382,}}, // n = 9
    {{0,53,125,215,320,}}, // n = 10
    {{0,35,88,160,250,}}, // n = 11
    {{0,20,55,108,180,}}, // n = 12
    {{0,10,30,65,118,}}, // n = 13
    {{0,4,14,34,69,}}, // n = 14
  }},
  {{ // i = 4
    {{0,0,0,0,0,}}, // n = 0
    {{0,25,0,0,0,}}, // n = 1
    {{0,25,50,0,0,}}, // n = 2
    {{0,25,50,75,0,}}, // n = 3
    {{0,25,50,75,100,}}, // n = 4
    {{0,25,50,75,100,}}, // n = 5
    {{0,25,50,75,100,}}, // n = 6
    {{0,25,50,75,100,}}, // n = 7
    {{0,24,49,74,99,}}, // n = 8
    {{0,22,46,71,96,}}, // n = 9
    {{0,19,41,65,90,}}, // n = 10
    {{0,15,34,56,80,}}, // n = 11
    {{0,10,25,44,66,}}, // n = 12
    {{0,6,16,31,50,}}, // n = 13
    {{0,3,9,19,34,}}, // n = 14
  }},
  {{ // i = 5
    {{0,0,0,0,0,}}, // n = 0
    {{0,5,0,0,0,}}, // n = 1
    {{0,5,10,0,0,}}, // n = 2
    {{0,5,10,15,0,}}, // n = 3
    {{0,5,10,15,20,}}, // n = 4
    {{0,5,10,15,20,}}, // n = 5
    {{0,5,10,15,20,}}, // n = 6
    {{0,5,10,15,20,}}, // n = 7
    {{0,5,10,15,20,}}, // n = 8
    {{0,5,10,15,20,}}, // n = 9
    {{0,5,10,15,20,}}, // n = 10
    {{0,5,10,15,20,}}, // n = 11
    {{0,4,9,14,19,}}, // n = 12
    {{0,3,7,12,17,}}, // n = 13
    {{0,2,5,9,14,}}, // n = 14
  }},
  {{ // i = 6
    {{0,0,0,0,0,}}, // n = 0
    {{0,1,0,0,0,}}, // n = 1
    {{0,1,2,0,0,}}, // n = 2
    {{0,1,2,3,0,}}, // n = 3
    {{0,1,2,3,4,}}, // n = 4
    {{0,1,2,3,4,}}, // n = 5
    {{0,1,2,3,4,}}, // n = 6
    {{0,1,2,3,4,}}, // n = 7
    {{0,1,2,3,4,}}, // n = 8
    {{0,1,2,3,4,}}, // n = 9
    {{0,1,2,3,4,}}, // n = 10
    {{0,1,2,3,4,}}, // n = 11
    {{0,1,2,3,4,}}, // n = 12
    {{0,1,2,3,4,}}, // n = 13
    {{0,1,2,3,4,}}, // n = 14
  }},
}};

} // namespace Nyanten::Standard_

#endif // !defined(NYANTEN_STANDARD_ZIPAI_TABLE_HPP_INCLUDE_GUARD)
