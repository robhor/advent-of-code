#include "day11.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day11;

std::string example_input = R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";

TEST(day11, ParseInput) {
  std::istringstream iss(example_input);
  Input input = parse_input(iss);
  EXPECT_EQ(input.galaxies,
            (std::vector<Vec2>{Vec2{3, 0}, Vec2{7, 1}, Vec2{0, 2}, Vec2{6, 4},
                               Vec2{1, 5}, Vec2{9, 6}, Vec2{7, 8}, Vec2{0, 9},
                               Vec2{4, 9}}));
  EXPECT_EQ(input.empty_x, (std::vector<int> {2, 5, 8}));
  EXPECT_EQ(input.empty_y, (std::vector<int> {3, 7}));
}

TEST(day11, ExpandUniverse) {
  std::istringstream iss(example_input);
  Input input = parse_input(iss);
  std::vector<Vec2> expanded = expand_universe(input);
  EXPECT_EQ(expanded,
            (std::vector<Vec2>{Vec2{4, 0}, Vec2{9, 1}, Vec2{0, 2}, Vec2{8, 5},
                               Vec2{1, 6}, Vec2{12, 7}, Vec2{9, 10}, Vec2{0, 11},
                               Vec2{5, 11}}));
}

TEST(day11, Dist) {
  std::istringstream iss(example_input);
  Input input = parse_input(iss);
  std::vector<Vec2> expanded = expand_universe(input);
  EXPECT_EQ(dist(expanded[0], expanded[6]), 15);
  EXPECT_EQ(dist(expanded[2], expanded[5]), 17);
  EXPECT_EQ(dist(expanded[7], expanded[8]), 5);
}

TEST(day11, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day11::part1(iss), 374);
}

}  // namespace