#include "day10.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

std::string example_input_1 = R"(.....
.S-7.
.|.|.
.L-J.
.....)";

std::string example_input_2 = R"(..F7.
.FJ|.
SJ.L7
|F--J
LJ...)";

using namespace AOC::day10;

TEST(day10, StartingPosition) {
  std::istringstream iss(example_input_1);
  AOC::day10::Map map = AOC::day10::parse_input(iss);
  EXPECT_EQ(map.find_starting_position(), (AOC::day10::Vec2 { 1, 1 }));
}

TEST(day10, Part1_1) {
  std::istringstream iss(example_input_1);
  EXPECT_EQ(AOC::day10::part1(iss), 4);
}

TEST(day10, Part1_2) {
  std::istringstream iss(example_input_2);
  EXPECT_EQ(AOC::day10::part1(iss), 8);
}

TEST(day10, Part2) {
  std::istringstream iss(example_input_1);
  EXPECT_EQ(AOC::day10::part2(iss), 0);
}

}  // namespace