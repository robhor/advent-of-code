#include "day05.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

  std::string example_input =
      R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)";

TEST(day05, ParseInput) {
  std::istringstream iss(example_input);
  AOC::day05::Input input = AOC::day05::parse_input(iss);
  EXPECT_EQ(input.seeds, (std::vector<int64_t>{79, 14, 55, 13}));
  EXPECT_EQ(input.mappings.seed_to_soil, (AOC::day05::Mapping {
    .range_mappings = {
      {50, 98, 2},
      {52, 50, 48}
    }
  }));
  EXPECT_EQ(input.mappings.humidity_to_location.range_mappings.size(), 2);
  EXPECT_EQ(input.mappings.humidity_to_location.range_mappings[0], (AOC::day05::RangeMapping {
    .destination_range_start = 60,
    .source_range_start = 56,
    .length = 37
  }));
}

TEST(day05, SeedToSoilMapping) {
  std::istringstream iss(example_input);
  AOC::day05::Input input = AOC::day05::parse_input(iss);
  EXPECT_EQ(input.mappings.seed_to_soil.map(79), 81);
  EXPECT_EQ(input.mappings.seed_to_soil.map(14), 14);
  EXPECT_EQ(input.mappings.seed_to_soil.map(55), 57);
  EXPECT_EQ(input.mappings.seed_to_soil.map(13), 13);
}

TEST(day05, SeedToLocationMapping) {
  std::istringstream iss(example_input);
  AOC::day05::Input input = AOC::day05::parse_input(iss);
  EXPECT_EQ(input.map_seed_to_location(79), 82);
  EXPECT_EQ(input.map_seed_to_location(14), 43);
  EXPECT_EQ(input.map_seed_to_location(55), 86);
  EXPECT_EQ(input.map_seed_to_location(13), 35);
}

TEST(day05, Part1) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day05::part1(iss), 35);
}

TEST(day05, RangeIntersection) {
  using AOC::day05::Range;

  Range r1 = {0, 10};
  Range r2 = {5, 10};
  Range r3 = {10, 10};

  EXPECT_TRUE(r1.intersects(r2));
  EXPECT_TRUE(r2.intersects(r1));
  EXPECT_FALSE(r1.intersects(r3));
  EXPECT_FALSE(r3.intersects(r1));
  EXPECT_TRUE(r2.intersects(r3));
  EXPECT_TRUE(r3.intersects(r2));
}

TEST(day05, MapRange_NoIntersection) {
  using AOC::day05::Range;
  using AOC::day05::RangeMapping;
  using AOC::day05::RangeMapResult;

  Range seeds = {0, 10};
  RangeMapping mapping = {0, 20, 10};

  EXPECT_EQ(mapping.map(seeds), (RangeMapResult{{}, {seeds}}));
}

TEST(day05, MapRange_StartIntersection) {
  using AOC::day05::Range;
  using AOC::day05::RangeMapping;
  using AOC::day05::RangeMapResult;

  Range seeds = {0, 10}; // {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
  RangeMapping mapping = {
    .destination_range_start = 100,
    .source_range_start = 8,
    .length = 5
    }; // 8 9 10 11 12 -> 100 101 102 103 104

  EXPECT_EQ(mapping.map(seeds), (RangeMapResult{{{100, 2}}, {{0, 8}}}));
  // {0, 1, 2, 3, 4, 5, 6, 7, 100, 101}
}

TEST(day05, MapRange_Intersection) {
  using AOC::day05::Range;
  using AOC::day05::RangeMapping;
  using AOC::day05::RangeMapResult;

  Range seeds = {0, 10}; // {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
  RangeMapping mapping = {
    .destination_range_start = 100,
    .source_range_start = 4,
    .length = 3
  }; // 4 5 6 -> 100 101 102

  // {0, 1, 2, 3, 100, 101, 103, 7, 8, 9}
  EXPECT_EQ(mapping.map(seeds), (RangeMapResult{{{100, 3}}, {{0, 4}, {7, 3}}}));
}

TEST(day05, Part2) {
  std::istringstream iss(example_input);
  EXPECT_EQ(AOC::day05::part2(iss), 46);
}

}  // namespace