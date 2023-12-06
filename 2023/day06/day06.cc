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
namespace day06 {

struct Race {
  int time;
  int distance;
};

std::vector<Race> parse_input(std::basic_istream<char>& in) {
  std::vector<Race> result;
  std::string line;
  
  std::getline(in, line);
  constexpr int prefix_length = std::string("Distance:").length();
  std::vector<std::string> times = absl::StrSplit(absl::string_view(line).substr(prefix_length), ' ', absl::SkipWhitespace());

  std::getline(in, line);
  std::vector<std::string> distances = absl::StrSplit(absl::string_view(line).substr(prefix_length), ' ', absl::SkipWhitespace());

  for (int i = 0; i < times.size(); i++) {
    Race race;
    absl::SimpleAtoi(times[i], &race.time);
    absl::SimpleAtoi(distances[i], &race.distance);
    result.push_back(race);
  }

  return result;
}

int ways_to_win(Race race) {
  double T = race.time;
  double R = race.distance + 1;
  return std::floor(T/2.0 + std::sqrt(T*T/4.0 - R)) - std::ceil(T/2.0 - std::sqrt(T*T/4.0 - R)) + 1;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<Race> races = parse_input(in);

  int product = 1;
  for (Race race : races) {
    product *= ways_to_win(race);
  }

  return product;
}

int64_t part2(std::basic_istream<char>& in) {
  return 0;
}

}  // namespace day06
}  // namespace AOC
