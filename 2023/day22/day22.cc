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
namespace day22 {

using ranges::views::filter;
using ranges::views::transform;

struct Vec3 {
  int x;
  int y;
  int z;

  static Vec3 ZERO;
  static Vec3 UP;
  static Vec3 DOWN;
  static Vec3 LEFT;
  static Vec3 RIGHT;
  static Vec3 FORWARD;
  static Vec3 BACK;

  auto operator<=>(const Vec3&) const = default;

  Vec3 operator+(const Vec3& other) const {
    return Vec3{x + other.x, y + other.y, z + other.z};
  }

  Vec3 operator-(const Vec3& other) const {
    return Vec3{x - other.x, y - other.y, z - other.z};
  }

  Vec3 operator*(int factor) const { return Vec3{x * factor, y * factor}; }

  Vec3 sign() const {
    return Vec3{(x == 0)  ? 0
                : (x < 0) ? -1
                          : 1,
                (y == 0)  ? 0
                : (y < 0) ? -1
                          : 1,
                (z == 0)  ? 0
                : (z < 0) ? -1
                          : 1};
  }
};

Vec3 Vec3::ZERO = Vec3{0, 0, 0};
Vec3 Vec3::UP = Vec3{0, 0, 1};
Vec3 Vec3::DOWN = Vec3{0, 0, -1};
Vec3 Vec3::LEFT = Vec3{-1, 0, 0};
Vec3 Vec3::RIGHT = Vec3{1, 0, 0};
Vec3 Vec3::FORWARD = Vec3{0, 1, 0};
Vec3 Vec3::BACK = Vec3{0, -1, 0};

std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
  os << "{";
  os << vec.x << " , ";
  os << vec.y << " , ";
  os << vec.z;
  os << "}";
  return os;
}

struct Block {
  Vec3 start;
  Vec3 end;

  auto operator<=>(const Block&) const = default;

  bool intersects(const Block& other) const {
    return (start.x <= other.end.x && end.x >= other.start.x &&
            start.y <= other.end.y && end.y >= other.start.y &&
            start.z <= other.end.z && end.z >= other.start.z);
  }

  Block operator+(const Vec3& dir) const {
    return Block { start + dir, end + dir };
  }
};

std::ostream& operator<<(std::ostream& os, const Block& block) {
  os << block.start << "~" << block.end;
  return os;
}

Vec3 parse_vec3(absl::string_view lv) {
  Vec3 result;
  std::vector<absl::string_view> coords = absl::StrSplit(lv, ",");
  assert(absl::SimpleAtoi(coords[0], &result.x));
  assert(absl::SimpleAtoi(coords[1], &result.y));
  assert(absl::SimpleAtoi(coords[2], &result.z));
  return result;
}

Block parse_block(absl::string_view line) {
  std::vector<absl::string_view> start_end = absl::StrSplit(line, "~");
  Vec3 start = parse_vec3(start_end[0]);
  Vec3 end = parse_vec3(start_end[1]);
  Block block{start, end};
  assert(start.x <= end.x);
  assert(start.y <= end.y);
  assert(start.z <= end.z);
  return block;
}

std::vector<Block> parse_input(std::basic_istream<char>& in) {
  std::vector<Block> blocks;
  std::string line;
  while (std::getline(in, line)) {
    blocks.push_back(parse_block(line));
  }
  return blocks;
}

std::vector<Block> let_gravity_do_its_thing(const std::vector<Block>& ordered_blocks) {
  std::deque<Block> falling_blocks = std::deque<Block>(ordered_blocks.begin(), ordered_blocks.end());
  std::vector<Block> fallen_blocks;

  while (!falling_blocks.empty()) {
    Block block = falling_blocks.front();
    falling_blocks.pop_front();

    while (block.start.z > 1) {
      Block falling = block + Vec3::DOWN;
      bool intersects = false;
      for (const auto& other : fallen_blocks | std::ranges::views::reverse) {
        if (falling.intersects(other)) {
          intersects = true;
          break;
        }
      }
      if (intersects) break;
      block = falling;
    }
    fallen_blocks.push_back(block);
  }

  return fallen_blocks;
}

// blocks resting on the argument
std::vector<Block> blocks_resting_above(const std::vector<Block>& blocks, const Block& block) {
  std::vector<Block> result;
  for (const Block& other : blocks) {
    if (block == other) continue;
    if (block.intersects(other + Vec3::DOWN)) {
      result.push_back(other);
    }
  }
  return result;
}

std::vector<Block> blocks_rested_on_below(const std::vector<Block>& blocks, const Block& block) {
  std::vector<Block> result;
  const Block fallen_self = block + Vec3::DOWN;
  for (const Block& other : blocks) {
    if (block == other) continue;
    if (other.intersects(fallen_self)) {
      result.push_back(other);
    }
  }
  return result;
}

int64_t count_disintegratable(const std::vector<Block>& blocks) {
  int64_t result = 0;
  for (const Block& block : blocks) {
    std::vector<Block> blocks_above = blocks_resting_above(blocks, block);
    int falling_above = 0;
    for (const Block& above : blocks_above) {
      const auto above_resting_blocks = blocks_rested_on_below(blocks, above);

      if (blocks_rested_on_below(blocks, above).size() <= 1) {
        falling_above++;
      }
    }
    if (falling_above == 0) {
      result++;
    }
  }
  return result;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<Block> blocks = parse_input(in);
  std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b){ return a.start.z < b.start.z; });

  // std::cout << ranges::views::all(blocks) << std::endl;

  std::vector<Block> fallen_blocks = let_gravity_do_its_thing(blocks);
  // std::cout << ranges::views::all(fallen_blocks) << std::endl;
  return count_disintegratable(fallen_blocks);
}

struct Node {
  std::vector<int> below;
  std::vector<int> above;
};

template <typename T>
int indexOf(const std::vector<T>& vec, const T& elem) {
  auto it = std::find(vec.begin(), vec.end(), elem);
  if (it == vec.end()) return -1;
  return std::distance(vec.begin(), it);
}

std::vector<Node> build_graph(const std::vector<Block>& blocks) {
  std::vector<Node> nodes;
  for (const Block& block : blocks) {
    nodes.push_back(Node());
  }

  for (int i = 0; i < blocks.size(); i++) {
    const Block& block = blocks[i];
    Node& node = nodes[i];

    for (const auto& above : blocks_resting_above(blocks, block)) {
      int j = indexOf(blocks, above);
      node.above.push_back(j);
    }

    for (const auto& above : blocks_rested_on_below(blocks, block)) {
      int j = indexOf(blocks, above);
      node.below.push_back(j);
    }
  }

  return nodes;
}

int64_t falling_blocks_if_disintegrating(std::vector<Node> graph, int start) {
  std::queue<int> removal_queue;
  removal_queue.push(start);
  int64_t removed = 0;
  while (!removal_queue.empty()) {
    int i = removal_queue.front();
    removal_queue.pop();
    removed++;

    for (int above : graph[i].above) {
      std::vector<int>& below = graph[above].below;
      below.erase(std::remove(below.begin(), below.end(), i), below.end());
      if (below.empty()) {
        removal_queue.push(above);
      }
    }
  }
  return removed - 1;
}

int64_t part2(std::basic_istream<char>& in) {
  std::vector<Block> blocks = parse_input(in);
  std::sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b){ return a.start.z < b.start.z; });
  std::vector<Block> fallen_blocks = let_gravity_do_its_thing(blocks);
  std::vector<Node> graph = build_graph(fallen_blocks);

  int64_t sum = 0;
  for (int i = 0; i < graph.size(); i++) {
    sum += falling_blocks_if_disintegrating(graph, i);
  }

  return sum;
}

int64_t part1(std::string in) {
  std::istringstream iss(in);
  return part1(iss);
}

int64_t part2(std::string in) {
  std::istringstream iss(in);
  return part2(iss);
}

}  // namespace day22
}  // namespace AOC
