#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace AOC {
namespace day05 {

struct RangeMapping {
  int64_t destination_range_start;
  int64_t source_range_start;
  int64_t length;

  auto operator<=>(const RangeMapping&) const = default;

  bool contains(int64_t source) const {
    return source >= source_range_start && source < source_range_start + length;
  }

  int64_t map(int64_t source) const {
    if (this->contains(source)) {
      int64_t idx = source - source_range_start;
      return destination_range_start + idx;
    }
    return source;
  }
};

struct Mapping {
  std::vector<RangeMapping> range_mappings;

  auto operator<=>(const Mapping&) const = default;

  int64_t map(int64_t source) const {
    for (const auto& range : range_mappings) {
      if (range.contains(source)) {
        return range.map(source);
      }
    }
    return source;
  }
};

struct Input {
  std::vector<int64_t> seeds;
  Mapping seed_to_soil;
  Mapping soil_to_fertilizer;
  Mapping fertilizer_to_water;
  Mapping water_to_light;
  Mapping light_to_temperature;
  Mapping temperature_to_humidity;
  Mapping humidity_to_location;

  auto operator<=>(const Input&) const = default;

  int64_t map_seed_to_location(int64_t seed) {
    int64_t result = seed;
    result = seed_to_soil.map(result);
    result = soil_to_fertilizer.map(result);
    result = fertilizer_to_water.map(result);
    result = water_to_light.map(result);
    result = light_to_temperature.map(result);
    result = temperature_to_humidity.map(result);
    result = humidity_to_location.map(result);
    return result;
  }
};

RangeMapping parse_range_mapping(absl::string_view line) {
  RangeMapping result;
  std::vector<absl::string_view> parts = absl::StrSplit(line, ' ');
  absl::SimpleAtoi(parts[0], &result.destination_range_start);
  absl::SimpleAtoi(parts[1], &result.source_range_start);
  absl::SimpleAtoi(parts[2], &result.length);
  return result;
}

Mapping parse_mapping(std::basic_istream<char>& in, Mapping& result) {
  std::string line;
  while (std::getline(in, line) && !line.empty()) {
    result.range_mappings.push_back(parse_range_mapping(line));
  }
  return result;
}


Input parse_input(std::basic_istream<char>& in) {
  Input result;
  std::string line;

  {
    // seeds line
    std::getline(in, line);
    absl::string_view seeds_line = absl::string_view(line).substr(std::strlen("seeds: "));
    std::vector<absl::string_view> seed_numbers = absl::StrSplit(seeds_line, ' ');
    std::transform(seed_numbers.begin(), seed_numbers.end(), std::back_inserter(result.seeds), [](absl::string_view num_str){
      int64_t num;
      absl::SimpleAtoi<int64_t>(num_str, &num);
      return num;
    });
  }

  // empty line
  std::getline(in, line);

  std::getline(in, line);
  assert(line == "seed-to-soil map:");
  parse_mapping(in, result.seed_to_soil);

  std::getline(in, line);
  assert(line == "soil-to-fertilizer map:");
  parse_mapping(in, result.soil_to_fertilizer);

  std::getline(in, line);
  assert(line == "fertilizer-to-water map:");
  parse_mapping(in, result.fertilizer_to_water);

  std::getline(in, line);
  assert(line == "water-to-light map:");
  parse_mapping(in, result.water_to_light);

  std::getline(in, line);
  assert(line == "light-to-temperature map:");
  parse_mapping(in, result.light_to_temperature);

  std::getline(in, line);
  assert(line == "temperature-to-humidity map:");
  parse_mapping(in, result.temperature_to_humidity);

  std::getline(in, line);
  assert(line == "humidity-to-location map:");
  parse_mapping(in, result.humidity_to_location);

  return result;
}

int64_t part1(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  
  int64_t min = INT64_MAX;
  for (int64_t seed : input.seeds) {
    min = std::min(min, input.map_seed_to_location(seed));
  }

  return min;
}

int64_t part2(std::basic_istream<char>& in) {
  return 0;
}

}  // namespace day05
}  // namespace AOC
