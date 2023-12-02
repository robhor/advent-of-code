#include <gtest/gtest.h>
#include <string>

#include "day01.cc"

TEST(Day01, Part1) {
    std::string example_input = R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)";

    std::istringstream iss(example_input);
    EXPECT_EQ(AOC::day01::part1(iss), "142");
}

TEST(Day01, Part2) {
    std::string example_input = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";

    std::istringstream iss(example_input);
    EXPECT_EQ(AOC::day01::part2(iss), "281");
}
