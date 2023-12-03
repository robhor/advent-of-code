#include "day02.cc"

#include <gtest/gtest.h>

#include <string>

namespace {
using AOC::day02::Game;
using AOC::day02::GameRound;

  std::string example_input =
      R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";


TEST(Day02, ParseInput) {
  std::string example_input =
      R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue)";

  Game game1{.id = 1,
             .rounds = {
                 {.blue = 3, .red = 4},
                 {.red = 1, .green = 2, .blue = 6},
                 {.green = 2},
             }};

  Game game2 {.id = 2,
             .rounds = {
                 {.blue = 1, .green = 2},
                 {.red = 1, .green = 3, .blue = 4},
                 {.green = 1, .blue = 1},
             }};

  std::vector<Game> expected = {game1, game2};

  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day02::parse_input(iss), expected);
}

TEST(Day02, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day02::part1(iss), 8);
}

TEST(Day02, MinSet) {
  std::istringstream iss(example_input);
  std::vector<Game> games = AOC::day02::parse_input(iss);

  EXPECT_EQ(AOC::day02::min_limit(games[0]), (GameRound {.red=4, .green=2, .blue=6}));
  EXPECT_EQ(AOC::day02::min_limit(games[1]), (GameRound {.red=1, .green=3, .blue=4}));
  EXPECT_EQ(AOC::day02::min_limit(games[2]), (GameRound {.red=20, .green=13, .blue=6}));
  EXPECT_EQ(AOC::day02::min_limit(games[3]), (GameRound {.red=14, .green=3, .blue=15}));
  EXPECT_EQ(AOC::day02::min_limit(games[4]), (GameRound {.red=6, .green=3, .blue=2}));
}

TEST(Day02, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day02::part2(iss), 2286);
}
}  // namespace