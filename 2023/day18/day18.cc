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
namespace day18 {

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

struct Vec2Hasher {
  std::size_t operator()(const Vec2& s) const {
    std::size_t h1 = s.x;
    std::size_t h2 = s.y;
    return h1 ^ (h2 << 1);
  }
};


std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
  os << "{";
  os << vec.x << " , ";
  os << vec.y;
  os << "}";
  return os;
}

struct DigInstruction {
  Vec2 direction;
  int distance;
  int color;
};

std::vector<DigInstruction> parse_input(std::basic_istream<char>& in) {
  std::vector<DigInstruction> result;
  std::string line;
  while (std::getline(in, line)) {
    DigInstruction instruction;
    std::vector<absl::string_view> parts = absl::StrSplit(line, " ");
    switch (parts[0][0]) {
      case 'R':
        instruction.direction = Vec2::RIGHT;
        break;
      case 'D':
        instruction.direction = Vec2::DOWN;
        break;
      case 'L':
        instruction.direction = Vec2::LEFT;
        break;
      case 'U':
        instruction.direction = Vec2::UP;
        break;
      default:
        assert(false && "Unknown direction");
    }

    assert(absl::SimpleAtoi(parts[1], &instruction.distance));
    assert(absl::SimpleHexAtoi(parts[2].substr(2, 6), &instruction.color));
    result.push_back(instruction);
  }
  return result;
}

std::unordered_map<Vec2, bool, Vec2Hasher> run_dig_instructions(std::vector<DigInstruction> instructions) {
  std::unordered_map<Vec2, bool, Vec2Hasher> dug_out;
  Vec2 current = Vec2::ZERO;
  dug_out[current] = true;

  for (const auto& instruction : instructions) {
    for (int i = 1; i <= instruction.distance; i++) {
      current = current + instruction.direction;
      dug_out[current] = true;
    }
  }

  return dug_out;
}

std::vector<Vec2> dig_instructions_to_vertices(const std::vector<DigInstruction>& instructions) {
  std::vector<Vec2> vertices;
  Vec2 current = Vec2::ZERO;
  vertices.push_back(current);

  for (const auto& instruction : instructions) {
    current = current + instruction.direction * instruction.distance;
    vertices.push_back(current);
  }

  return vertices;
}

int64_t area_of_polygon(const std::vector<DigInstruction>& instructions) {
  // shoelace formula
  std::vector<Vec2> vertices = dig_instructions_to_vertices(instructions);
  int64_t sum = 0;
  int n = vertices.size();

  auto x = [&vertices](int i) { return vertices[i].x; };
  auto y = [&vertices](int i) { return vertices[i].y; };

  for (int i = 0; i < n; i++) {
    sum += x(i) * y((i + 1) % n);
    sum -= x((i + 1) % n) * y(i);
  }

  const int64_t inner_area = sum/2;

  int64_t outline_length = 0;
  for (const auto& i : instructions) {
    outline_length += i.distance;
  }

  return inner_area + outline_length/2 + 1;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<DigInstruction> instructions = parse_input(in);
  return area_of_polygon(instructions);
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

}  // namespace day18
}  // namespace AOC
