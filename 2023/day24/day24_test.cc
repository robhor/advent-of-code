#include "day24.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day24;

std::string example_input = R"(19, 13, 30 @ -2,  1, -2
18, 19, 22 @ -1, -1, -2
20, 25, 34 @ -2, -2, -4
12, 31, 28 @ -1, -2, -1
20, 19, 15 @  1, -5, -3)";

TEST(day24, Part1) {
  EXPECT_EQ(part1(example_input, Range { 7, 27 }), 2);
}

}  // namespace