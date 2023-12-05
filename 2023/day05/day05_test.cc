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
  EXPECT_EQ(input.seed_to_soil, (AOC::day05::Mapping {
    .range_mappings = {
      {50, 98, 2},
      {52, 50, 48}
    }
  }));
  EXPECT_EQ(input.humidity_to_location.range_mappings.size(), 2);
  EXPECT_EQ(input.humidity_to_location.range_mappings[0], (AOC::day05::RangeMapping {
    .destination_range_start = 60,
    .source_range_start = 56,
    .length = 37
  }));
}

TEST(day05, SeedToSoilMapping) {
  std::istringstream iss(example_input);
  AOC::day05::Input input = AOC::day05::parse_input(iss);
  EXPECT_EQ(input.seed_to_soil.map(79), 81);
  EXPECT_EQ(input.seed_to_soil.map(14), 14);
  EXPECT_EQ(input.seed_to_soil.map(55), 57);
  EXPECT_EQ(input.seed_to_soil.map(13), 13);
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

// TEST(day05, Part2) {
//   std::istringstream iss(example_input);
//   EXPECT_EQ(AOC::day05::part2(iss), 30);
// }

}  // namespace