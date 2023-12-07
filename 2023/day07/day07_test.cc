#include "day07.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

std::string example_input = R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";

using namespace AOC::day07;

TEST(day07, ParseInput) {
  std::istringstream iss(example_input);
  std::vector<HandBid> input = parse_input(iss);
  EXPECT_EQ(input.size(), 5);
  EXPECT_EQ(input[0].hand, (Hand{3, 2, 10, 3, 13}));
  EXPECT_EQ(input[0].bid, 765);
  EXPECT_EQ(input[1].hand, (Hand{10, 5, 5, 11, 5}));
  EXPECT_EQ(input[1].bid, 684);
}

TEST(day07, HandKind) {
  EXPECT_EQ(kind_of(parse_hand("AAAAA")), FIVE_OF_A_KIND);
  EXPECT_EQ(kind_of(parse_hand("AA8AA")), FOUR_OF_A_KIND);
  EXPECT_EQ(kind_of(parse_hand("23332")), FULL_HOUSE);
  EXPECT_EQ(kind_of(parse_hand("TTT98")), THREE_OF_A_KIND);
  EXPECT_EQ(kind_of(parse_hand("23432")), TWO_PAIR);
  EXPECT_EQ(kind_of(parse_hand("A23A4")), ONE_PAIR);
  EXPECT_EQ(kind_of(parse_hand("23456")), HIGH_CARD);
}

TEST(day07, StrongerThan) {
  EXPECT_TRUE(is_stronger_than(parse_hand("AAAAA"), parse_hand("AA8AA")));
  EXPECT_TRUE(is_stronger_than(parse_hand("23332"), parse_hand("A23A4")));
  EXPECT_TRUE(is_stronger_than(parse_hand("33332"), parse_hand("2AAAA")));
  EXPECT_TRUE(is_stronger_than(parse_hand("77888"), parse_hand("77788")));

  EXPECT_FALSE(is_stronger_than(parse_hand("AA8AA"), parse_hand("AAAAA")));
  EXPECT_FALSE(is_stronger_than(parse_hand("A23A4"), parse_hand("23332")));
  EXPECT_FALSE(is_stronger_than(parse_hand("2AAAA"), parse_hand("33332")));
  EXPECT_FALSE(is_stronger_than(parse_hand("77788"), parse_hand("77888")));

  EXPECT_TRUE(is_stronger_than(parse_hand("A5AA9"), parse_hand("TJAJT")));
  EXPECT_FALSE(is_stronger_than(parse_hand("TJAJT"), parse_hand("A5AA9")));
  EXPECT_TRUE(is_stronger_than(parse_hand("4J444"), parse_hand("44JJ4")));
  EXPECT_FALSE(is_stronger_than(parse_hand("44JJ4"), parse_hand("4J444")));
}

TEST(day07, RankHands) {
  std::istringstream iss(example_input);
  std::vector<HandBid> hands = parse_input(iss);
  rank_hands(hands);
  EXPECT_EQ(hands[0].hand, parse_hand("32T3K"));
  EXPECT_EQ(hands[1].hand, parse_hand("KTJJT"));
  EXPECT_EQ(hands[2].hand, parse_hand("KK677"));
  EXPECT_EQ(hands[3].hand, parse_hand("T55J5"));
  EXPECT_EQ(hands[4].hand, parse_hand("QQQJA"));
}

TEST(day07, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day07::part1(iss), 6440);
}

TEST(day07, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day07::part2(iss), 0);
}

}  // namespace