#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <ranges>
#include <set>
#include <span>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/all.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/split.hpp"

namespace AOC {
namespace day14 {

using ranges::views::filter;
using ranges::views::transform;

std::vector<std::string> parse_input(std::basic_istream<char>& in) {
  std::vector<std::string> result;
  std::string line;
  while (std::getline(in, line)) {
    result.push_back(line);
  }
  return result;
}

std::vector<std::string> tilt_north(const std::vector<std::string>& input) {
  std::vector<std::string> result = input;
  for (int x = 0; x < input[0].size(); x++) {
    int last_base = -1;
    for (int y = 0; y < input.size(); y++) {
      char here = result[y][x];
      if (here == '.') continue;
      if (here == '#') {
        last_base = y;
        continue;
      }
      if (here == 'O') {
        int target = last_base + 1;
        last_base = target;
        result[target][x] = 'O';
        if (y != target) {
          result[y][x] = '.';
        }
      }
    }
  }
  return result;
}

int load(const std::vector<std::string>& input) {
  int load = 0;
  int depth = input.size();
  for (const auto& line : input) {
    for (const char c : line) {
      if (c == 'O') load += depth;
    }
    depth--;
  }
  return load;
}

int64_t part1(std::basic_istream<char>& in) {
  return load(tilt_north(parse_input(in)));
}

int64_t part2(std::basic_istream<char>& in) {
  return 0;
}

int64_t part1(std::string in) {
  std::istringstream iss(in);
  return part1(iss);
}

int64_t part2(std::string in) {
  std::istringstream iss(in);
  return part2(iss);
}

}  // namespace day14
}  // namespace AOC
