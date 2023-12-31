#include "day08.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

std::string example_input_1 = R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";

std::string example_input_2 = R"(LLR

AAA = (BBB, BBB)
BBB = (AAA, ZZZ)
ZZZ = (ZZZ, ZZZ))";

std::string example_input_3 = R"(LR

11A = (11B, XXX)
11B = (XXX, 11Z)
11Z = (11B, XXX)
22A = (22B, XXX)
22B = (22C, 22C)
22C = (22Z, 22Z)
22Z = (22B, 22B)
XXX = (XXX, XXX))";

using namespace AOC::day08;

TEST(day08, Part1) {
  std::istringstream iss(example_input_1);
  EXPECT_EQ(AOC::day08::part1(iss), 2);

  std::istringstream iss2(example_input_2);
  EXPECT_EQ(AOC::day08::part1(iss2), 6);
}

TEST(day08, Part2) {
  std::istringstream iss(example_input_3);
  EXPECT_EQ(AOC::day08::part2(iss), 6);
}

}  // namespace