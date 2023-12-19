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
  std::string color_hex;
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
    instruction.color_hex = parts[2].substr(2, 6);
    result.push_back(instruction);
  }
  return result;
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

uint64_t area_of_polygon(const std::vector<DigInstruction>& instructions) {
  // shoelace formula
  std::vector<Vec2> vertices = dig_instructions_to_vertices(instructions);
  uint64_t sum = 0;
  int n = vertices.size();

  auto x = [&vertices](int i) { return (uint64_t) vertices[i].x; };
  auto y = [&vertices](int i) { return (uint64_t) vertices[i].y; };

  for (int i = 0; i < n; i++) {
    sum += x(i) * y((i + 1) % n);
    sum -= x((i + 1) % n) * y(i);
  }

  const uint64_t inner_area = sum/2LL;
  assert(inner_area > 0);

  uint64_t outline_length = 0;
  for (const auto& i : instructions) {
    outline_length += i.distance;
  }

  return inner_area + outline_length/2LL + 1LL;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<DigInstruction> instructions = parse_input(in);
  return area_of_polygon(instructions);
}

std::vector<DigInstruction> decode_color_instructions(const std::vector<DigInstruction>& instructions) {
  std::vector<DigInstruction> result;
  for (const auto& instruction : instructions) {
    DigInstruction decoded;
    assert(absl::SimpleHexAtoi(instruction.color_hex.substr(0, 5), &decoded.distance));
    switch (instruction.color_hex[5]) {
    case '0':
      decoded.direction = Vec2::RIGHT;
      break;
    case '1':
      decoded.direction = Vec2::DOWN;
      break;
    case '2':
      decoded.direction = Vec2::LEFT;
      break;
    case '3':
      decoded.direction = Vec2::UP;
      break;
    default:
      assert(false && "Invalid direction code");
    }
    result.push_back(decoded);
  }
  return result;
}

int64_t part2(std::basic_istream<char>& in) {
  std::vector<DigInstruction> instructions = parse_input(in);
  std::vector<DigInstruction> decoded_instructions = decode_color_instructions(instructions);
  return area_of_polygon(decoded_instructions);
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
