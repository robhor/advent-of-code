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
namespace day16 {

using ranges::views::filter;
using ranges::views::transform;

const int MIRROR_NW_SE = 1 << 0; // 1
const int MIRROR_NE_SW = 1 << 1; // 2
const int VERTICAL_SPLITTER = 1 << 2; // 4
const int HORIZONTAL_SPLITTER = 1 << 3; // 8
const int VISITED_N = 1 << 4; // 16
const int VISITED_W = 1 << 5; // 32
const int VISITED_S = 1 << 6; // 64
const int VISITED_E = 1 << 7; // 128

struct Position {
  int x;
  int y;
  int heading;

  Position north() const {
    return Position {x, y - 1, VISITED_N};
  }

  Position south() const {
    return Position {x, y + 1, VISITED_S};
  }

  Position west() const {
    return Position {x - 1, y, VISITED_W};
  }

  Position east() const {
    return Position {x + 1, y, VISITED_E};
  }
};

struct Level {
  std::vector<std::vector<int>> map;
  int visited = 0;

  bool is_valid(const Position& p) {
    if (p.x < 0) return false;
    if (p.y < 0) return false;
    if (p.x >= map[0].size()) return false;
    if (p.y >= map.size()) return false;
    return true;
  }

  std::vector<Position> visit(const Position& p) {
    if (!is_valid(p)) return {};
    if ((map[p.y][p.x] & p.heading) != 0) return {};
    if ((map[p.y][p.x] & (VISITED_N | VISITED_W | VISITED_S | VISITED_E)) == 0) visited++;
    map[p.y][p.x] |= p.heading;
    
    char tile = map[p.y][p.x];
    if ((tile & MIRROR_NW_SE) != 0) {
      // mirror: '\'
      if (p.heading == VISITED_N) return {p.west()};
      if (p.heading == VISITED_W) return {p.north()};
      if (p.heading == VISITED_S) return {p.east()};
      if (p.heading == VISITED_E) return {p.south()};
    } else if ((tile & MIRROR_NE_SW) != 0) {
      // mirror: /
      if (p.heading == VISITED_N) return {p.east()};
      if (p.heading == VISITED_W) return {p.south()};
      if (p.heading == VISITED_S) return {p.west()};
      if (p.heading == VISITED_E) return {p.north()};
    } else if ((tile & VERTICAL_SPLITTER) != 0) {
      // splitter: |
      if (p.heading == VISITED_N) return {p.north()};
      if (p.heading == VISITED_S) return {p.south()};
      if (p.heading == VISITED_W) return {p.north(), p.south()};
      if (p.heading == VISITED_E) return {p.north(), p.south()};
    } else if ((tile & HORIZONTAL_SPLITTER) != 0) {
      // splitter: -
      if (p.heading == VISITED_N) return {p.west(), p.east()};
      if (p.heading == VISITED_S) return {p.west(), p.east()};
      if (p.heading == VISITED_W) return {p.west()};
      if (p.heading == VISITED_E) return {p.east()};
    } else {
      // empty space: .
      if (p.heading == VISITED_N) return {p.north()};
      if (p.heading == VISITED_W) return {p.west()};
      if (p.heading == VISITED_S) return {p.south()};
      if (p.heading == VISITED_E) return {p.east()};
    }

    assert(false && "Nowhere to go");
  }

  int trace_beam(const Position& start) {
    std::deque<Position> queue;
    queue.push_back(start);

    while (!queue.empty()) {
      const Position pos = queue.front();
      queue.pop_front();

      std::vector<Position> next = visit(pos);
      for (const auto& p : next) queue.push_back(p);
    }

    return visited;
  }
};

Level parse_input(std::basic_istream<char>& in) {
  Level level;
  std::string line;
  while (std::getline(in, line)) {
    std::vector<int> row;
    for (char c : line) {
      if (c == '.') row.push_back(0);
      else if (c == '-') row.push_back(HORIZONTAL_SPLITTER);
      else if (c == '|') row.push_back(VERTICAL_SPLITTER);
      else if (c == '\\') row.push_back(MIRROR_NW_SE);
      else if (c == '/') row.push_back(MIRROR_NE_SW);
      else {
        printf("Unknown input: %c\n", c);
        assert(false);
      }
    }
    level.map.push_back(row);
  }
  return level;
}

int64_t part1(std::basic_istream<char>& in) {
  Level level = parse_input(in);
  return level.trace_beam(Position{0, 0, VISITED_E});
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

}  // namespace day16
}  // namespace AOC
