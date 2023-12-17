#include "day17.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day17;

std::string example_input = R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533)";

TEST(day17, Part1) {
  EXPECT_EQ(part1(example_input), 102);
}

TEST(day17, Part2) {
  EXPECT_EQ(part2(example_input), 0);
}

}  // namespace