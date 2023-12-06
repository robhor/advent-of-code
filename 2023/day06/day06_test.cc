#include "day06.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

std::string example_input = R"(Time:      7  15   30
Distance:  9  40  200)";

TEST(day06, ParseInput) {
  std::istringstream iss(example_input);
  std::vector<AOC::day06::Race> input = AOC::day06::parse_input(iss);
  EXPECT_EQ(input.size(), 3);
  EXPECT_EQ(input[0].time, 7);
  EXPECT_EQ(input[0].distance, 9);
  EXPECT_EQ(input[1].time, 15);
  EXPECT_EQ(input[1].distance, 40);
  EXPECT_EQ(input[2].time, 30);
  EXPECT_EQ(input[2].distance, 200);
}

TEST(day06, WaysToWin) {
  std::istringstream iss(example_input);
  std::vector<AOC::day06::Race> races = AOC::day06::parse_input(iss);
  EXPECT_EQ(AOC::day06::ways_to_win(races[0]), 4);
  EXPECT_EQ(AOC::day06::ways_to_win(races[1]), 8);
  EXPECT_EQ(AOC::day06::ways_to_win(races[2]), 9);
}

TEST(day06, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day06::part1(iss), 288);
}

TEST(day06, ParseInput2) {
  std::istringstream iss(example_input);
  AOC::day06::Race input = AOC::day06::parse_as_single_race(iss);
  EXPECT_EQ(input.time, 71530);
  EXPECT_EQ(input.distance, 940200);
}

TEST(day06, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day06::part2(iss), 71503);
}

}  // namespace