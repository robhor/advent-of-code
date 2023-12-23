#include "day22.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day22;

std::string example_input = R"(1,0,1~1,2,1
0,0,2~2,0,2
0,2,3~2,2,3
0,0,4~0,2,4
2,0,5~2,2,5
0,1,6~2,1,6
1,1,8~1,1,9)";

TEST(day22, Part1) {
  EXPECT_EQ(part1(example_input), 5);
}

TEST(day22, Part2) {
  EXPECT_EQ(part2(example_input), 7);
}

}  // namespace