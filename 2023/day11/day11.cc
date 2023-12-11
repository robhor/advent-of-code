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
#include <tuple>
#include <numeric>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "range/v3/view/all.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/numeric/accumulate.hpp"


namespace AOC {
namespace day11 {

using ranges::views::filter;
using ranges::views::transform;

struct Vec2 {
  int64_t x;
  int64_t y;

  static Vec2 UP;
  static Vec2 DOWN;
  static Vec2 LEFT;
  static Vec2 RIGHT;

  auto operator<=>(const Vec2&) const = default;

  Vec2 operator+(const Vec2& other) const {
    return Vec2 { x + other.x, y + other.y };
  }
};

Vec2 Vec2::UP = Vec2 {0, -1};
Vec2 Vec2::DOWN = Vec2 {0, 1};
Vec2 Vec2::LEFT = Vec2 {-1, 0};
Vec2 Vec2::RIGHT = Vec2 {1, 0};

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
  os << "{";
  os << vec.x << " , ";
  os << vec.y;
  os << "}";
  return os;
}

struct Input {
  std::vector<Vec2> galaxies;
  std::vector<int> empty_x;
  std::vector<int> empty_y;
};

Input parse_input(std::basic_istream<char>& in) {
  std::vector<std::string> lines;
  {
    std::string line;
    while(std::getline(in, line)) {
      lines.push_back(line);
    }
  }

  Input result;
  for (int y = 0; y < lines.size(); y++) {
    std::string& line = lines[y];
    bool is_empty = true;
    for (int x = 0; x < line.size(); x++) {
      char c = line[x];
      if (c == '.') continue;
      is_empty = false;
      result.galaxies.push_back(Vec2 {x, y});
    }
    if (is_empty) {
      result.empty_y.push_back(y);
    }
  }
  for (int x = 0; x < lines[0].size(); x++) {
    bool is_empty = std::none_of(result.galaxies.begin(), result.galaxies.end(), [x](Vec2 v) { return v.x == x; });
    if (is_empty) {
      result.empty_x.push_back(x);
    }
  }

  return result;
}

std::vector<Vec2> expand_universe(const Input& input, int expansion_factor = 2) {
  std::vector<Vec2> result = input.galaxies;
  for (int ey : std::views::reverse(input.empty_y)) {
    for (Vec2& v : result) {
      if (v.y > ey) v.y += expansion_factor - 1;
    }
  }
  for (int ex : std::views::reverse(input.empty_x)) {
    for (Vec2& v : result) {
      if (v.x > ex) v.x += expansion_factor - 1;
    }
  }
  return result;
}

int64_t dist(Vec2 a, Vec2 b) {
  return std::abs(b.x - a.x) + std::abs(b.y - a.y);
}

int64_t calculate_distances(const Input& input, int expansion_factor) {
  std::vector<Vec2> expanded = expand_universe(input, expansion_factor);

  int64_t sum = 0;
  for (int i = 0; i < expanded.size(); i++) {
    for (int j = i + 1; j < expanded.size(); j++) {
      sum += dist(expanded[i], expanded[j]);
    }
  }
  return sum;
}

int64_t part1(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  return calculate_distances(input, 2);
}

int64_t part2(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  return calculate_distances(input, 1000000);
}

}  // namespace day11
}  // namespace AOC
