#include "day18.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day18;

std::string example_input = R"(R 6 (#70c710)
D 5 (#0dc571)
L 2 (#5713f0)
D 2 (#d2c081)
R 2 (#59c680)
D 2 (#411b91)
L 5 (#8ceee2)
U 2 (#caa173)
L 1 (#1b58a2)
U 2 (#caa171)
R 2 (#7807d2)
U 3 (#a77fa3)
L 2 (#015232)
U 2 (#7a21e3))";

TEST(day18, Part1) {
  EXPECT_EQ(part1(example_input), 62);
}

TEST(day18, Part2) {
  EXPECT_EQ(part2(example_input), 0);
}

}  // namespace