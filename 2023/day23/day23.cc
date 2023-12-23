#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <queue>
#include <ranges>
#include <set>
#include <unordered_set>
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

struct Vec2Hasher {
  std::size_t operator()(const Vec2& p) const {
    std::size_t h1 = p.x;
    std::size_t h2 = p.y;
    return h1 ^ (h2 << 1);
  }
};

struct MapNode {
  Vec2 pos;
  std::vector<std::pair<MapNode*, int64_t>> next {};
};

struct MapNodeHasher {
  std::size_t operator()(const MapNode& n) const {
    std::size_t h1 = n.pos.x;
    std::size_t h2 = n.pos.y;
    return h1 ^ (h2 << 1);
  }
};

struct Node {
  Vec2 last { Vec2::ZERO };
  std::unordered_set<Vec2, Vec2Hasher> visited {};
};

MapNode* compress_graph(MapNode* start) {
  std::deque<MapNode*> open;
  std::unordered_set<Vec2, Vec2Hasher> closed;
  std::vector<MapNode*> pruned;
  open.push_back(start);

  while (!open.empty()) {
    MapNode* node = open.front();
    open.pop_front();
    if (closed.contains(node->pos)) continue;
    closed.insert(node->pos);

    for (const auto& neighbor : node->next) {
      open.push_back(neighbor.first);
    }

    if (node->next.size() == 2) {
      // can be pruned: replaced by direct edge between neighbors
      int64_t weight = 0;
      weight += node->next[0].second;
      weight += node->next[1].second;

      MapNode* prev = node->next[0].first;
      MapNode* next = node->next[1].first;
      prev->next.erase(std::remove_if(prev->next.begin(), prev->next.end(), [=](const auto& link){ return link.first == node; }), prev->next.end());
      next->next.erase(std::remove_if(next->next.begin(), next->next.end(), [=](const auto& link){ return link.first == node; }), next->next.end());
      prev->next.push_back({ next, weight });
      next->next.push_back({ prev, weight });

      node->next.clear();
      pruned.push_back(node);
    }
  }

  for (const MapNode* node : pruned) {
    delete node;
  }

  return start;
}

MapNode* construct_graph(const std::vector<std::string>& map, bool steep_slopes = true) {
  Vec2 start = Vec2 {(int) map[0].find('.') , 0};
  Vec2 dest = Vec2 {(int) map.back().find('.') , (int) map.size() - 1};

  MapNode* start_node = new MapNode { start };
  std::unordered_map<Vec2, MapNode*, Vec2Hasher> pos_to_node;
  pos_to_node[start] = start_node;

  std::deque<MapNode*> queue;
  queue.push_back(start_node);

  while (!queue.empty()) {
    MapNode* n = queue.front();
    queue.pop_front();
    Vec2 last = n->pos;

    std::vector<Vec2> neighbors;
    char tile = map[last.y][last.x];
    if (steep_slopes && tile == '<') {
      neighbors.push_back(last + Vec2::LEFT);
    } else if (steep_slopes && tile == '>') {
      neighbors.push_back(last + Vec2::RIGHT);
    } else if (steep_slopes && tile == 'v') {
      neighbors.push_back(last + Vec2::DOWN);
    } else if (steep_slopes && tile == '^') {
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
      return true;
    }) | ranges::to<std::vector>();

    for (const Vec2& neighbor : neighbors) {
      MapNode* next = pos_to_node[neighbor];
      if (next == nullptr) {
        next = new MapNode;
        next->pos = neighbor;
        pos_to_node[neighbor] = next;
        queue.push_back(next);
      }
      n->next.push_back({next, 1});
    }
  }

  return compress_graph(start_node);
}

int64_t find_longest_path(const MapNode* start, Vec2 dest) {
  struct SearchNode {
    const MapNode* current { nullptr };
    int64_t path_length { 0LL };
    std::vector<Vec2> visited {};
  };

  std::deque<SearchNode> queue;
  SearchNode start_node {start};
  start_node.visited.push_back(start->pos);
  queue.push_back(start_node);

  int64_t max = 0;

  while (!queue.empty()) {
    SearchNode n = queue.front();
    queue.pop_front();
    Vec2 last = n.current->pos;

    if (last == dest) {
      max = std::max(max, n.path_length);
      continue;
    }

    std::vector<std::pair<MapNode*, int64_t>> neighbors = n.current->next | filter([&n](const auto& neighbor) {
      return std::find(n.visited.begin(), n.visited.end(), neighbor.first->pos) == n.visited.end();
    }) | ranges::to<std::vector>();

    for (const auto& neighbor : neighbors) {
      SearchNode next;
      next.current = neighbor.first;
      next.path_length = n.path_length + neighbor.second;
      next.visited = n.visited;
      next.visited.push_back(neighbor.first->pos);
      queue.push_back(next);
    }
  }

  return max;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<std::string> map = parse_input(in);
  MapNode* node = construct_graph(map, true);
  Vec2 dest = Vec2 {(int) map.back().find('.') , (int) map.size() - 1};
  return find_longest_path(node, dest);
}

int64_t part2(std::basic_istream<char>& in) {
  std::vector<std::string> map = parse_input(in);
  MapNode* node = construct_graph(map, false);
  Vec2 dest = Vec2 {(int) map.back().find('.') , (int) map.size() - 1};
  return find_longest_path(node, dest);
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
