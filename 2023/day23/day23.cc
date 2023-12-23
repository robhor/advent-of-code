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
namespace day23 {

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

struct Node {
  std::vector<Vec2> path;
};

int64_t find_longest_path(const std::vector<std::string>& map) {
  Vec2 start = Vec2 {(int) map[0].find('.') , 0};
  Vec2 dest = Vec2 {(int) map.back().find('.') , (int) map.size() - 1};

  std::deque<Node> queue;

  Node start_node;
  start_node.path.push_back(start);
  queue.push_back(start_node);
  int64_t max = 0;

  while (!queue.empty()) {
    Node n = queue.front();
    queue.pop_front();
    Vec2 last = n.path.back();

    if (last == dest) {
      max = std::max(max, (int64_t) n.path.size());
      continue;
    }

    std::vector<Vec2> neighbors;
    char tile = map[last.y][last.x];
    if (tile == '<') {
      neighbors.push_back(last + Vec2::LEFT);
    } else if (tile == '>') {
      neighbors.push_back(last + Vec2::RIGHT);
    } else if (tile == 'v') {
      neighbors.push_back(last + Vec2::DOWN);
    } else if (tile == '^') {
      neighbors.push_back(last + Vec2::UP);
    } else {
      neighbors.push_back(last + Vec2::UP);
      neighbors.push_back(last + Vec2::DOWN);
      neighbors.push_back(last + Vec2::LEFT);
      neighbors.push_back(last + Vec2::RIGHT);
    }

    neighbors = neighbors | filter([&map, &n](const Vec2& p){
      if (p.x < 0) return false;
      if (p.y < 0) return false;
      if (p.x >= map[0].size()) return false;
      if (p.y >= map.size()) return false;
      if (map[p.y][p.x] == '#') return false;
      return std::find(n.path.begin(), n.path.end(), p) == n.path.end();
    }) | ranges::to<std::vector>();

    for (const Vec2& neighbor : neighbors) {
      Node next;
      next.path = n.path;
      next.path.push_back(neighbor);
      queue.push_back(next);
    }
  }

  return max - 1;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<std::string> map = parse_input(in);
  return find_longest_path(map);
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

}  // namespace day23
}  // namespace AOC
