#include "day09.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

std::string example_input = R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45)";

using namespace AOC::day09;

TEST(day09, Extrapolate) {
  std::istringstream iss(example_input);
  auto histories = AOC::day09::parse_input(iss);
  EXPECT_EQ(AOC::day09::extrapolate(histories[0]), 18);
  EXPECT_EQ(AOC::day09::extrapolate(histories[1]), 28);
  EXPECT_EQ(AOC::day09::extrapolate(histories[2]), 68);
}

TEST(day09, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day09::part1(iss), 114);
}

TEST(day09, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day09::part2(iss), 0);
}

}  // namespace