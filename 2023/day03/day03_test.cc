#include "day03.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

  std::string example_input =
      R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..)";


TEST(day03, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day03::part1(iss), 4361);
}

TEST(day03, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day03::part2(iss), 467835);
}
}  // namespace