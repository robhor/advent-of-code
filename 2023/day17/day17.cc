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
#include "range/v3/view/transform.hpp"
#include "range/v3/view/split.hpp"

namespace AOC {
namespace day17 {

using ranges::views::filter;
using ranges::views::transform;

typedef std::vector<std::vector<int>> Map;

int HEADING_UNDEFINED = 0;
int HEADING_NORTH = 1;
int HEADING_WEST = 2;
int HEADING_SOUTH = 3;
int HEADING_EAST = 4;

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
    return Vec2 { x + other.x, y + other.y };
  }

  Vec2 operator-(const Vec2& other) const {
    return Vec2 { x - other.x, y - other.y };
  }

  Vec2 operator*(int factor) const {
    return Vec2 { x * factor, y * factor };
  }

  Vec2 sign() const {
    return Vec2 {
      (x == 0) ? 0 : (x < 0) ? -1 : 1,
      (y == 0) ? 0 : (y < 0) ? -1 : 1
    };
  }
};

Vec2 Vec2::ZERO = Vec2 {0, 0};
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

struct Node {
  Vec2 position;
  int heading;

  auto operator<=>(const Node&) const = default;
};

Map parse_input(std::basic_istream<char>& in) {
  std::vector<std::vector<int>> result;
  std::string line;
  while (std::getline(in, line)) {
    std::vector<int> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    result.push_back(row);
  }
  return result;
}

int sdist(const Vec2& left, const Vec2& right) {
  return (left.x - right.x) * (left.x - right.x) + (left.y - right.y) * (left.y - right.y);
}

struct NodeHasher {
  std::size_t operator()(const Node& s) const {
    std::size_t h1 = s.position.x;
    std::size_t h2 = s.position.y;
    std::size_t h3 = s.heading;
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

bool in_bounds(const Map& map, const Vec2& pos) {
  if (pos.x < 0) return false;
  if (pos.y < 0) return false;
  if (pos.x >= map[0].size()) return false;
  if (pos.y >= map.size()) return false;
  return true;
}

std::vector<Node> neighbors(const Map& map, const Node& node) {
  std::vector<Node> neighbors;
  Vec2 pos = node.position;
  if (node.heading == HEADING_WEST || node.heading == HEADING_EAST) {
    neighbors.push_back(Node { pos + Vec2::UP, HEADING_NORTH });;
    neighbors.push_back(Node { pos + Vec2::UP * 2, HEADING_NORTH });;
    neighbors.push_back(Node { pos + Vec2::UP * 3, HEADING_NORTH });;
  }

  if (node.heading == HEADING_NORTH || node.heading == HEADING_SOUTH || node.heading == HEADING_UNDEFINED) {
    neighbors.push_back(Node { pos + Vec2::LEFT, HEADING_WEST });;
    neighbors.push_back(Node { pos + Vec2::LEFT * 2, HEADING_WEST });;
    neighbors.push_back(Node { pos + Vec2::LEFT * 3, HEADING_WEST });;
  }

  if (node.heading == HEADING_WEST || node.heading == HEADING_EAST || node.heading == HEADING_UNDEFINED) {
    neighbors.push_back(Node { pos + Vec2::DOWN, HEADING_SOUTH });;
    neighbors.push_back(Node { pos + Vec2::DOWN * 2, HEADING_SOUTH });;
    neighbors.push_back(Node { pos + Vec2::DOWN * 3, HEADING_SOUTH });;
  }

  if (node.heading == HEADING_NORTH || node.heading == HEADING_SOUTH) {
    neighbors.push_back(Node { pos + Vec2::RIGHT, HEADING_EAST });;
    neighbors.push_back(Node { pos + Vec2::RIGHT * 2, HEADING_EAST });;
    neighbors.push_back(Node { pos + Vec2::RIGHT * 3, HEADING_EAST });;
  }

  return neighbors | filter([&map](const Node& n) { return in_bounds(map, n.position); }) | ranges::to<std::vector>();
}

int cost(const Map& map, Vec2 pos_start, Vec2 pos_end) {
  int cost = 0;
  Vec2 dir = (pos_end - pos_start).sign();
  for (Vec2 p = pos_start + dir; p != pos_end; p = p + dir) {
    cost += map[p.y][p.x];
  }
  cost += map[pos_end.y][pos_end.x];
  return cost;
}

int dijkstra(const Map& map) {
  Vec2 goal = Vec2 { (int) (map[0].size() - 1), (int) (map.size() - 1) };
  std::unordered_map<Node, Node, NodeHasher> came_from;
  std::unordered_map<Node, int, NodeHasher> cost_so_far;

  auto cmp = [&cost_so_far](const Node& left, const Node& right) { return cost_so_far[left] > cost_so_far[right]; };
  std::priority_queue<Node, std::vector<Node>, decltype(cmp)> frontier(cmp);

  Node start { Vec2::ZERO, HEADING_UNDEFINED };
  frontier.push(start);

  
  cost_so_far[start] = 0;

  while (!frontier.empty()) {
    Node current = frontier.top();
    if (current.position == goal) {
      break;
    }
    frontier.pop();

    for (Node next : neighbors(map, current)) {
      int new_cost = cost_so_far[current] + cost(map, current.position, next.position);
      if (!cost_so_far.contains(next) || new_cost < cost_so_far[next]) {
          cost_so_far[next] = new_cost;
          frontier.push(next);
          came_from[next] = current;
      }
    }
  }

  Node yay = frontier.top();
  int shortest_path_cost = cost_so_far[yay];

  std::vector<Vec2> path;
  {
    // reconstruct path
    Node current = yay;
    while (current.position != start.position) {
      path.push_back(current.position);
      current = came_from[current];
    }
    path.push_back(start.position);
  }

  {
    // calculate path cost
    int costt = 0;
    Vec2 current = path.back();
    for (int i = path.size() - 2; i >= 0; i--) {
      Vec2 next = path[i];
      costt += cost(map, current, next);
      current = next;
    }
    std::cout << "Min path cost: " << costt << std::endl;
  }

  std::cout << "Min path: " << ranges::views::all(path) << std::endl;

  return shortest_path_cost;
}

int64_t part1(std::basic_istream<char>& in) {
  Map map = parse_input(in);
  return dijkstra(map);
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

}  // namespace day17
}  // namespace AOC
