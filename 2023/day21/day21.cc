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
        return Vec2{x, y};
      }
    }
  }
  assert(false && "Starting pos not found");
}

int mod(int a, int b) { return (b + (a % b)) % b; }

std::set<Vec2> take_a_step(const std::vector<std::string>& map,
                           std::set<Vec2> positions, bool bounded = true) {
  std::set<Vec2> new_positions;
  std::vector<Vec2> directions = {Vec2::UP, Vec2::DOWN, Vec2::LEFT,
                                  Vec2::RIGHT};
  int w = map[0].size();
  int h = map.size();
  for (Vec2 p : positions) {
    for (Vec2 d : directions) {
      Vec2 new_position = p + d;
      if (bounded) {
        if (new_position.x < 0) continue;
        if (new_position.y < 0) continue;
        if (new_position.x >= map[0].size()) continue;
        if (new_position.y >= map.size()) continue;
      }
      if (map[mod(new_position.y, h)][mod(new_position.x, w)] != '#') {
        new_positions.insert(new_position);
      }
    }
  }
  return new_positions;
}

std::set<Vec2> take_a_walk(const std::vector<std::string>& map, int steps,
                           Vec2 start, bool bounded = true) {
  std::set<Vec2> end_positions;
  end_positions.insert(start);

  for (int step = 0; step < steps; step++) {
    end_positions = take_a_step(map, end_positions, bounded);
  }

  return end_positions;
}

std::set<Vec2> take_a_walk(const std::vector<std::string>& map, int steps) {
  Vec2 start = find_starting_pos(map);
  return take_a_walk(map, steps, start);
}

int64_t end_positions_in_a_walk(const std::vector<std::string>& map, int steps,
                                Vec2 start) {
  return take_a_walk(map, steps, start).size();
}

int64_t part1(std::basic_istream<char>& in, int steps = 64) {
  std::vector<std::string> map = parse_input(in);
  return take_a_walk(map, steps).size();
}

int64_t part2(std::basic_istream<char>& in) {
  // This only works because of the special structure of the input
  std::vector<std::string> map = parse_input(in);

  int h = map.size();
  int w = h;
  Vec2 start = find_starting_pos(map);

  assert(start.x == (w - 1) / 2);
  assert(start.y == (h - 1) / 2);

  int64_t D = 26501365LL;
  int64_t N = (D - start.x) / w;
  int64_t rem = D - N * w;
  printf("w=%d, h=%d\n", w, h);
  printf("D=%lld\n", D);
  printf("start.x=%d\n", start.x);
  printf("N=%lld\n", N);
  assert(N == 202300);

  int64_t E = end_positions_in_a_walk(map, 3 * w + 1, start);
  int64_t O = end_positions_in_a_walk(map, 3 * w, start);

  int64_t sA = w + rem - 1;
  int64_t A1 = end_positions_in_a_walk(map, sA, Vec2{w - 1, h - 1});
  int64_t A2 = end_positions_in_a_walk(map, sA, Vec2{0, h - 1});
  int64_t A3 = end_positions_in_a_walk(map, sA, Vec2::ZERO);
  int64_t A4 = end_positions_in_a_walk(map, sA, Vec2{w - 1, 0});
  int64_t A = A1 + A2 + A3 + A4;

  int64_t sB = rem - 1;
  int64_t B1 = end_positions_in_a_walk(map, sB, Vec2{w - 1, h - 1});
  int64_t B2 = end_positions_in_a_walk(map, sB, Vec2{0, h - 1});
  int64_t B3 = end_positions_in_a_walk(map, sB, Vec2::ZERO);
  int64_t B4 = end_positions_in_a_walk(map, sB, Vec2{w - 1, 0});
  int64_t B = B1 + B2 + B3 + B4;

  int64_t sT = w - 1;
  int64_t T1 = end_positions_in_a_walk(map, sT, Vec2{w - 1, start.y});
  int64_t T2 = end_positions_in_a_walk(map, sT, Vec2{start.x, h - 1});
  int64_t T3 = end_positions_in_a_walk(map, sT, Vec2{0, start.y});
  int64_t T4 = end_positions_in_a_walk(map, sT, Vec2{start.x, 0});
  int64_t T = T1 + T2 + T3 + T4;

  int64_t reachable_tiles =
      (N - 1) * (N - 1) * O + N * N * E + (N - 1) * A + N * B + T;

  printf("part2: %lld\n", reachable_tiles);
  printf("E: %lld\n", E);
  printf("O: %lld\n", O);
  printf("A: %lld, A1=%lld, A2=%lld, A3=%lld, A4=%lld\n", A, A1, A2, A3, A4);
  printf("B: %lld, B1=%lld, B2=%lld, B3=%lld, B4=%lld\n", B, B1, B2, B3, B4);
  printf("T: %lld, T1=%lld, T2=%lld, T3=%lld, T4=%lld\n", T, T1, T2, T3, T4);
  // printf("part1: %lld\n", take_a_walk(map, D, start, false).size());

  return reachable_tiles;
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
