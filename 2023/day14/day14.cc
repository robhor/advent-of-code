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

struct PlatformView {
  int rotation = 0;
  std::vector<std::string>& input;

  PlatformView(std::vector<std::string>& input) : input(input) {}
  PlatformView(const PlatformView& input) : rotation(input.rotation), input(input.input) {}

  int width() const {
    if (rotation == 1 || rotation == 2) {
      return input.size();
    }
    return input[0].size();
  }

  int height() const {
    if (rotation == 1 || rotation == 2) {
      return input[0].size();
    }
    return input.size();
  }

  int getX(int x, int y) const {
    if (rotation == 0) return x;
    if (rotation == 1) return y;
    if (rotation == 2) return input[0].size() - 1 - x;
    if (rotation == 3) return input[0].size() - 1 - y;
  }

  int getY(int x, int y) const {
    if (rotation == 0) return y;
    if (rotation == 1) return input.size() - 1 - x;
    if (rotation == 2) return input.size() - 1 - y;
    if (rotation == 3) return x;
  }

  char get(int x, int y) const {
    return input[getY(x, y)][getX(x, y)];
  }

  void set(int x, int y, char c) {
    input[getY(x, y)][getX(x, y)] = c;
  }

  void rotate_right() {
    rotation = (rotation + 1) % 4;
  }
};

void tilt_north(PlatformView input) {
  for (int x = 0; x < input.width(); x++) {
    int last_base = -1;
    for (int y = 0; y < input.height(); y++) {
      char here = input.get(x, y);
      if (here == '.') continue;
      if (here == '#') {
        last_base = y;
        continue;
      }
      if (here == 'O') {
        int target = last_base + 1;
        last_base = target;
        input.set(x, target, 'O');
        if (y != target) {
          input.set(x, y, '.');
        }
      }
    }
  }
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
  std::vector<std::string> input = parse_input(in);
  tilt_north(PlatformView(input));
  return load(input);
}

int64_t hashof(const std::vector<std::string>& input) {
  int64_t hash = 0;
  for (const auto& line : input) {
    hash ^= std::hash<std::string>{}(line);
  }
  return hash;
}

int64_t part2(std::basic_istream<char>& in) {
  std::vector<std::string> input = parse_input(in);
  PlatformView platform(input);

  std::unordered_map<int64_t, int64_t> seen_at;
  int64_t cycles = 1000000000;
  for (int64_t i = 0; i < cycles; i++) {
    for (int j = 0; j < 4; j++) {
      tilt_north(platform);
      platform.rotate_right();
    }
    int64_t hash = hashof(input);
    if (seen_at.contains(hash)) {
      int64_t last_seen = seen_at[hash];
      int64_t cycles_since_last_seen = i - last_seen;
      while (i + cycles_since_last_seen < cycles-1) {
        i += cycles_since_last_seen;
      }
    }
    seen_at[hash] = i;
  }
  return load(input);
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
