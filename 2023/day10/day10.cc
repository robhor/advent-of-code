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
namespace day10 {

using ranges::views::filter;
using ranges::views::transform;

struct Vec2 {
  int x;
  int y;

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

struct Map {
  std::vector<std::string> rows;

  int x_size() const {
    return rows[0].size();
  }
  

  int y_size() const {
    return rows.size();
  }

  char get(int x, int y) const {
    if (x < 0 || x >= x_size()) return '.';
    if (y < 0 || y >= y_size()) return '.';
    return rows[y][x];
  }

  char get(Vec2 p) const {
    return get(p.x, p.y);
  }

  void set(Vec2 p, char v) {
    rows[p.y][p.x] = v;
  }

  Vec2 find_starting_position() const {
    for (int y = 0; y < y_size(); y++) {
      for (int x = 0; x < x_size(); x++) {
        if (get(x, y) == 'S') {
          return Vec2 {x, y};
        }
      }
    }
    assert(false && "Starting position not found");
  }
};

Map parse_input(std::basic_istream<char>& in) {
  std::vector<std::string> lines;
  std::string line;
  while(std::getline(in, line)) {
    lines.push_back(line);
  }

  return Map { lines };
}

std::optional<Vec2> next_direction(char tile, Vec2 direction) {
  if (direction == Vec2::UP) {
    if (tile == '|') return Vec2::UP;
    if (tile == 'F') return Vec2::RIGHT;
    if (tile == '7') return Vec2::LEFT;
    return std::nullopt;
  }
  if (direction == Vec2::RIGHT) {
    if (tile == 'J') return Vec2::UP;
    if (tile == '-') return Vec2::RIGHT;
    if (tile == '7') return Vec2::DOWN;
    return std::nullopt;
  }
  if (direction == Vec2::DOWN) {
    if (tile == '|') return Vec2::DOWN;
    if (tile == 'L') return Vec2::RIGHT;
    if (tile == 'J') return Vec2::LEFT;
    return std::nullopt;
  }
  if (direction == Vec2::LEFT) {
    if (tile == 'F') return Vec2::DOWN;
    if (tile == 'L') return Vec2::UP;
    if (tile == '-') return Vec2::LEFT;
    return std::nullopt;
  }
  assert(false && "Invalid direction");
}

class PipeRunner {
  public:
  const Map& map;
  Vec2 position;
  Vec2 direction; // facing
  std::vector<Vec2> visited_positions;

  PipeRunner(const Map& map, Vec2 position, Vec2 direction) : map(map), position(position), direction(direction) {
    visited_positions.push_back(position);
  }

  // From current position, head in direction
  bool advance() {
    position = position + direction;
    const std::optional<Vec2> new_direction = next_direction(map.get(position), direction);
    if (!new_direction.has_value()) return false;
    direction = new_direction.value();
    visited_positions.push_back(position);
    return true;
  }
};

std::optional<std::vector<Vec2>> find_pipe_loop(const Map& map, Vec2 start_pos, Vec2 start_direction) {
  PipeRunner runner = PipeRunner(map, start_pos, start_direction);
  while (runner.advance()) {
  }
  if (map.get(runner.position) == 'S') {
    return runner.visited_positions;
  }
  return std::nullopt;
}

std::vector<Vec2> find_pipe_loop(const Map& map) {
  Vec2 starting_position = map.find_starting_position();
  std::optional<std::vector<Vec2>> loop;
  loop = find_pipe_loop(map, starting_position, Vec2::UP);
  if (loop.has_value()) return loop.value();
  loop = find_pipe_loop(map, starting_position, Vec2::DOWN);
  if (loop.has_value()) return loop.value();
  loop = find_pipe_loop(map, starting_position, Vec2::LEFT);
  if (loop.has_value()) return loop.value();
  loop = find_pipe_loop(map, starting_position, Vec2::RIGHT);
  if (loop.has_value()) return loop.value();
  assert(false && "Have to find a loop!");
}

int64_t part1(std::basic_istream<char>& in) {
  Map map = parse_input(in);
  std::vector<Vec2> loop = find_pipe_loop(map);
  //std::cout << ranges::views::all(loop) << std::endl;

  int mid = loop.size() / 2;
  return mid;
}

void get_bounds(const std::vector<Vec2>& path, Vec2& out_min, Vec2& out_max) {
  out_min.x = INT_MAX;
  out_min.y = INT_MAX;
  out_max.x = 0;
  out_max.y = 0;

  for (const Vec2& p : path) {
    out_min.x = std::min(out_min.x, p.x);
    out_min.y = std::min(out_min.y, p.y);
    out_max.x = std::max(out_max.x, p.x);
    out_max.y = std::max(out_max.y, p.y);
  }
}

char get_tile_type(const std::vector<Vec2>& path, Vec2 pos) {
  Vec2 start = path[0];
  Vec2 after = path[1];
  Vec2 before = path.back();

  // F-7
  // |.|
  // L-J

  Vec2 min, max;
  get_bounds(std::vector<Vec2>{before, start, after}, min, max);

  if (after.x == before.x) return '|';
  if (after.y == before.y) return '-';
  if (start == min) return 'F';
  if (start == max) return 'J';
  if (start == Vec2{min.x, max.y}) return 'L';
  if (start == Vec2{max.x, min.y}) return '7';
  assert(false && "Unknown tile type");
}

int get_enclosed_spaces(Map map, std::vector<Vec2> path) {
  Map marked_map = map;
  for (const Vec2& p : path) {
    marked_map.set(p, '*');
  }

  Vec2 starting_pos = map.find_starting_position();
  map.set(starting_pos, get_tile_type(path, starting_pos));

  Vec2 min, max;
  get_bounds(path, min, max);

  int count = 0;
  for (int y = min.y; y <= max.y; y++) {
    for (int x = min.x; x <= max.x; x++) {
      if (marked_map.get(x,y) == '*') continue;
      int walls = 0;
      for (int xe = x + 1; xe <= max.x; xe++) {
        if (marked_map.get(xe, y) == '*') {
          char tile = map.get(xe, y);
          if (tile == '|') {
            walls++;
          } else if (tile == 'F') {
            xe++;
            while(map.get(xe,y) == '-') { xe++; }
            if (map.get(xe, y) == '7') { continue; }
            if (map.get(xe, y) == 'J') {
              walls++;
              continue;
            }
          } else if (tile == 'L') {
            xe++;
            while(map.get(xe,y) == '-') { xe++; }
            if (map.get(xe, y) == 'J') { continue; }
            if (map.get(xe, y) == '7') {
              walls++;
              continue;
            }
          }
        }
      }
      bool inside = walls % 2 == 1;
      if (inside) count++;
    }
  }
  return count;
}

int64_t part2(std::basic_istream<char>& in) {
  Map map = parse_input(in);
  std::vector<Vec2> loop = find_pipe_loop(map);
  return get_enclosed_spaces(map, loop);
}

}  // namespace day10
}  // namespace AOC
