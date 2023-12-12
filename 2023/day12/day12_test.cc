#include "day12.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day12;

static std::stringstream operator "" _f(const char* const s, const size_t _) {
    return std::stringstream() << s;
}

std::string example_input = R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

TEST(day12, CanPlaceGroupAt) {
  EXPECT_EQ(can_place_group_at(0, 1, "???"), true);
  EXPECT_EQ(can_place_group_at(0, 3, "???"), true);
  EXPECT_EQ(can_place_group_at(0, 4, "???"), false);
  EXPECT_EQ(can_place_group_at(1, 3, "???"), false);
  EXPECT_EQ(can_place_group_at(0, 1, ".??"), false);
  EXPECT_EQ(can_place_group_at(1, 1, ".??"), true);
  EXPECT_EQ(can_place_group_at(1, 1, "#??"), false);
  EXPECT_EQ(can_place_group_at(1, 1, ".?."), true);
  EXPECT_EQ(can_place_group_at(1, 1, ".?#"), false);
  EXPECT_EQ(can_place_group_at(2, 1, "#??"), false);
  EXPECT_EQ(can_place_group_at(1, 3, "?##."), false);
}

TEST(day12, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day12::part1("???.### 1,1,3"), 1);
  EXPECT_EQ(AOC::day12::part1("?#?#?#?#?#?#?#? 1,3,1,6"), 1);
  EXPECT_EQ(AOC::day12::part1(".??..??...?##. 1,1,3"), 4);
  EXPECT_EQ(AOC::day12::part1("????.#...#... 4,1,1"), 1);
  EXPECT_EQ(AOC::day12::part1("????.######..#####. 1,6,5"), 4);
  EXPECT_EQ(AOC::day12::part1("?###???????? 3,2,1"), 10);
  EXPECT_EQ(AOC::day12::part1(example_input), 21);
}

TEST(day12, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(part2(iss), 0);
}

}  // namespace