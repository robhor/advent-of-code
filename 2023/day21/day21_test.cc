#include "day21.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day21;

std::string example_input = R"(...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........)";

TEST(day21, Part1_SixSteps) {
  std::istringstream iss(example_input);
  EXPECT_EQ(part1(iss, 6), 16);
}

}  // namespace