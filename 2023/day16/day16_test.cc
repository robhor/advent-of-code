#include "day16.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day16;

std::string example_input = R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....)";

TEST(day16, Part1) {
  EXPECT_EQ(part1(example_input), 46);
}

TEST(day16, Part2) {
  EXPECT_EQ(part2(example_input), 0);
}

}  // namespace