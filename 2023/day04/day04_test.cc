#include "day04.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

  std::string example_input =
      R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";

TEST(day04, ParseInput) {
  std::istringstream iss(example_input);
  auto scratchcards = AOC::day04::parse_input(iss);
  EXPECT_EQ(scratchcards[0], (AOC::day04::Scratchcard {
    .winning_numbers = {41, 48, 83, 86, 17},
    .actual_numbers = {83, 86, 6, 31, 17, 9, 48, 53},
  }));
}

TEST(day04, WinningNumbers) {
  std::istringstream iss(example_input);
  auto scratchcards = AOC::day04::parse_input(iss);
  EXPECT_EQ(scratchcards[0].matching_numbers(), std::vector<int>({48, 83, 86, 17}));
}

TEST(day04, Points) {
  std::istringstream iss(example_input);
  auto scratchcards = AOC::day04::parse_input(iss);
  EXPECT_EQ(scratchcards[0].points(), 8);
  EXPECT_EQ(scratchcards[1].points(), 2);
  EXPECT_EQ(scratchcards[2].points(), 2);
  EXPECT_EQ(scratchcards[3].points(), 1);
  EXPECT_EQ(scratchcards[4].points(), 0);
  EXPECT_EQ(scratchcards[5].points(), 0);
}

TEST(day04, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day04::part1(iss), 13);
}

TEST(day04, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day04::part2(iss), 30);
}

}  // namespace