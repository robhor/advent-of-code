#include <deque>
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

struct Range {
  int64_t start;
  int64_t length;

  auto operator<=>(const Range&) const = default;

  int64_t end() const { return start + length - 1; }

  bool intersects(const Range& other) const {
    if (start > other.start + other.length - 1) return false;
    if (other.start > start + length - 1) return false;
    if (start + length - 1 < other.start) return false;
    if (other.start + other.length - 1 < start) return false;
    return true;
  }

  Range intersection(const Range& other) const {
    int64_t intersection_start = std::max(start, other.start);
    int64_t intersection_end = std::min(this->end(), other.end());
    return Range{intersection_start, intersection_end - intersection_start + 1};
  }
};

std::ostream& operator<<(std::ostream& os, const Range& range) {
  os << "{" << range.start << ", " << range.length << "}";
  return os;
}

struct RangeMapResult {
  std::vector<Range> mapped_ranges;
  std::vector<Range> unmapped_ranges;

  auto operator<=>(const RangeMapResult&) const = default;
};

struct RangeMapping {
  int64_t destination_range_start;
  int64_t source_range_start;
  int64_t length;

  auto operator<=>(const RangeMapping&) const = default;

  bool contains(int64_t source) const {
    return source >= source_range_start && source < source_range_start + length;
  }

  bool intersects(const Range& range) const {
    Range mapping_range{source_range_start, length};
    return mapping_range.intersects(range);
  }

  int64_t map(int64_t source) const {
    if (this->contains(source)) {
      int64_t idx = source - source_range_start;
      return destination_range_start + idx;
    }
    return source;
  }

  RangeMapResult map(Range source_range) const {
    RangeMapResult result;
    Range mapping_range{source_range_start, length};
    if (!source_range.intersects(mapping_range)) {
      result.unmapped_ranges.push_back(source_range);
      return result;
    }
    Range intersection = source_range.intersection(mapping_range);

    if (intersection.start > source_range.start) {
      result.unmapped_ranges.push_back(
          Range{source_range.start, intersection.start - source_range.start});
    }
    result.mapped_ranges.push_back(Range{
        destination_range_start + (intersection.start - mapping_range.start),
        intersection.length});

    if (intersection.end() < source_range.end()) {
      result.unmapped_ranges.push_back(Range{
          intersection.end() + 1, source_range.end() - intersection.end()});
    }

    return result;
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

  std::vector<Range> map(const std::vector<Range>& source_ranges) {
    std::deque<Range> open_unmapped(source_ranges.begin(), source_ranges.end());
    std::vector<Range> unmapped_ranges;
    std::vector<Range> result;

    while (!open_unmapped.empty()) {
      Range source_range = open_unmapped.front();
      open_unmapped.pop_front();

      bool mapped = false;
      for (const RangeMapping& mapping : range_mappings) {
        if (mapping.intersects(source_range)) {
          RangeMapResult range_result = mapping.map(source_range);
          result.insert(result.end(), range_result.mapped_ranges.begin(),
                        range_result.mapped_ranges.end());
          for (const Range& range : range_result.unmapped_ranges) {
            open_unmapped.push_back(range);
          }
          mapped = true;
          break;
        }
      }
      if (!mapped) {
        result.push_back(source_range);
      }
    }

    return result;
  }
};

struct Mappings {
  Mapping seed_to_soil;
  Mapping soil_to_fertilizer;
  Mapping fertilizer_to_water;
  Mapping water_to_light;
  Mapping light_to_temperature;
  Mapping temperature_to_humidity;
  Mapping humidity_to_location;

  auto operator<=>(const Mappings&) const = default;

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
  std::vector<Range> map_seed_ranges_to_location(
      const std::vector<Range>& seeds) {
    std::vector<Range> result = seeds;
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

struct Input {
  std::vector<int64_t> seeds;
  Mappings mappings;

  auto operator<=>(const Input&) const = default;

  int64_t map_seed_to_location(int64_t seed) {
    return this->mappings.map_seed_to_location(seed);
  }
};

struct Input2 {
  std::vector<Range> seeds;
  Mappings mappings;

  auto operator<=>(const Input2&) const = default;

  int64_t map_seed_to_location(int64_t seed) {
    return this->mappings.map_seed_to_location(seed);
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

Mappings parse_mappings(std::basic_istream<char>& in, Mappings& result) {
  std::string line;
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

Input parse_input(std::basic_istream<char>& in) {
  Input result;
  std::string line;

  {
    // seeds line
    std::getline(in, line);
    absl::string_view seeds_line =
        absl::string_view(line).substr(std::strlen("seeds: "));
    std::vector<absl::string_view> seed_numbers =
        absl::StrSplit(seeds_line, ' ');
    std::transform(seed_numbers.begin(), seed_numbers.end(),
                   std::back_inserter(result.seeds),
                   [](absl::string_view num_str) {
                     int64_t num;
                     absl::SimpleAtoi<int64_t>(num_str, &num);
                     return num;
                   });
  }

  std::getline(in, line);  // consume empty line
  parse_mappings(in, result.mappings);

  return result;
}

Input2 parse_input2(std::basic_istream<char>& in) {
  Input2 result;
  std::string line;

  {
    // seeds line
    std::getline(in, line);
    absl::string_view seeds_line =
        absl::string_view(line).substr(std::strlen("seeds: "));
    std::vector<absl::string_view> seed_numbers =
        absl::StrSplit(seeds_line, ' ');

    for (int i = 0; i + 1 < seed_numbers.size(); i += 2) {
      int64_t start_seed;
      int64_t length;
      absl::SimpleAtoi(seed_numbers[i], &start_seed);
      absl::SimpleAtoi(seed_numbers[i + 1], &length);
      result.seeds.push_back(Range{start_seed, length});
    }
  }

  std::getline(in, line);  // consume empty line
  parse_mappings(in, result.mappings);

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
  Input2 input = parse_input2(in);
  std::vector<Range> mapped_ranges = input.mappings.map_seed_ranges_to_location(input.seeds);

  int64_t min = INT64_MAX;
  for (Range range : mapped_ranges) {
    min = std::min(min, range.start);
  }

  return min;
}

}  // namespace day05
}  // namespace AOC
