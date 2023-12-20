#include "day20.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day20;

std::string example_input1 = R"(broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a)";

std::string example_input2 = R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output)";

TEST(day20, Part1_example1) {
  EXPECT_EQ(part1(example_input1), 32000000);
}

TEST(day20, Part1_example2) {
  EXPECT_EQ(part1(example_input2), 11687500);
}

TEST(day20, Part2) {
  EXPECT_EQ(part2(example_input1), 0);
}

}  // namespace