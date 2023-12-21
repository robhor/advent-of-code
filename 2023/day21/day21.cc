#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <queue>
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
#include "range/v3/view/split.hpp"
#include "range/v3/view/transform.hpp"

namespace AOC {
namespace day21 {

using ranges::views::filter;
using ranges::views::transform;

struct Vec2 {
  int x;
  int y;

  static Vec2 ZERO;
  static Vec2 UP;
  static Vec2 DOWN;
  static Vec2 LEFT;
  static Vec2 RIGHT;

  auto operator<=>(const Vec2&) const = default;

  Vec2 operator+(const Vec2& other) const {
    return Vec2{x + other.x, y + other.y};
  }

  Vec2 operator-(const Vec2& other) const {
    return Vec2{x - other.x, y - other.y};
  }

  Vec2 operator*(int factor) const { return Vec2{x * factor, y * factor}; }

  Vec2 sign() const {
    return Vec2{(x == 0)  ? 0
                : (x < 0) ? -1
                          : 1,
                (y == 0)  ? 0
                : (y < 0) ? -1
                          : 1};
  }
};

Vec2 Vec2::ZERO = Vec2{0, 0};
Vec2 Vec2::UP = Vec2{0, -1};
Vec2 Vec2::DOWN = Vec2{0, 1};
Vec2 Vec2::LEFT = Vec2{-1, 0};
Vec2 Vec2::RIGHT = Vec2{1, 0};

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
  os << "{";
  os << vec.x << " , ";
  os << vec.y;
  os << "}";
  return os;
}

std::vector<std::string> parse_input(std::basic_istream<char>& in) {
  std::vector<std::string> map;
  std::string line;
  while (std::getline(in, line)) {
    map.push_back(line);
  }
  return map;
}

Vec2 find_starting_pos(const std::vector<std::string>& map) {
  for (int y = 0; y < map.size(); y++) {
    for (int x = 0; x < map[0].size(); x++) {
      if (map[y][x] == 'S') {
        return Vec2 { x, y };
      }
    }
  }
  assert(false && "Starting pos not found");
}

std::set<Vec2> take_a_step(const std::vector<std::string>& map, std::set<Vec2> positions) {
  std::set<Vec2> new_positions;
  std::vector<Vec2> directions = { Vec2::UP, Vec2::DOWN, Vec2::LEFT, Vec2::RIGHT };
  for (Vec2 p : positions) {
    for (Vec2 d : directions) {
      Vec2 new_position = p + d;
      if (map[new_position.y][new_position.x] != '#') {
        new_positions.insert(new_position);
      }
    }
  }
  return new_positions;
}

std::set<Vec2> take_a_walk(const std::vector<std::string>& map, int steps) {
  std::set<Vec2> end_positions;
  end_positions.insert(find_starting_pos(map));

  for (int step = 0; step < steps; step++) {
    end_positions = take_a_step(map, end_positions);
  }

  return end_positions;
}

int64_t part1(std::basic_istream<char>& in, int steps = 64) {
  std::vector<std::string> map = parse_input(in);
  return take_a_walk(map, steps).size();
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

}  // namespace day21
}  // namespace AOC
