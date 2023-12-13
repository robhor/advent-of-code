#include "day13.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day13;

std::string example_input = R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)";

TEST(day13, Part1) {
  EXPECT_EQ(part1(R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.)"), 5);
EXPECT_EQ(part1(R"(#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)"), 400);
  EXPECT_EQ(part1(example_input), 405);
}

TEST(day13, Part2) {
  EXPECT_EQ(part2(example_input), 0);
}

}  // namespace