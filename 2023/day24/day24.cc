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
namespace day24 {

using ranges::views::filter;
using ranges::views::transform;

struct Vec3 {
  double x;
  double y;
  double z;

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

  Vec3 operator*(double factor) const { return Vec3{x * factor, y * factor}; }

  Vec3 sign() const {
    return Vec3{(x == 0.0)  ? 0.0
                : (x < 0.0) ? -1.0
                          : 1.0,
                (y == 0.0)  ? 0.0
                : (y < 0.0) ? -1.0
                          : 1.0,
                (z == 0.0)  ? 0.0
                : (z < 0.0) ? -1.0
                          : 1.0};
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

Vec3 parse_vec3(absl::string_view lv) {
  Vec3 result;
  std::vector<absl::string_view> coords = absl::StrSplit(lv, ", ");
  assert(absl::SimpleAtod(coords[0], &result.x));
  assert(absl::SimpleAtod(coords[1], &result.y));
  assert(absl::SimpleAtod(coords[2], &result.z));
  return result;
}

struct Hailstone {
  Vec3 position;
  Vec3 velocity;
};

std::ostream& operator<<(std::ostream& os, const Hailstone& hailstone) {
  os << hailstone.position << " @ " << hailstone.velocity;
  return os;
}

struct Range {
  int64_t at_least;
  int64_t at_most;

  bool includes(double t) {
    return t >= at_least && t <= at_most;
  }
};

std::vector<Hailstone> parse_input(std::basic_istream<char>& in) {
  std::vector<Hailstone> result;
  std::string line;
  while (std::getline(in, line)) {
    absl::string_view linev(line);
    std::vector<absl::string_view> pos_vel = absl::StrSplit(linev, " @ ");
    Vec3 pos = parse_vec3(pos_vel[0]);
    Vec3 vel = parse_vec3(pos_vel[1]);
    result.push_back(Hailstone { pos, vel });
  }
  return result;
}

Vec3 intersect_2d(const Hailstone& a, const Hailstone& b) {
    // Line AB represented as a1x + b1y = c1
    double a1 = (a.position + a.velocity).y - a.position.y;
    double b1 = (a.position).x - (a.position + a.velocity).x;
    double c1 = a1*(a.position.x) + b1*(a.position.y);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = (b.position + b.velocity).y - b.position.y;
    double b2 = b.position.x - (b.position + b.velocity).x;
    double c2 = a2*(b.position.x)+ b2*(b.position.y);
 
    double determinant = a1*b2 - a2*b1;
 
    if (determinant == 0) {
        // The lines are parallel.
        return Vec3 { 0, 0, -1 };
    } else {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return Vec3 { x, y, 0.0 };
    }
}

int64_t part1(std::basic_istream<char>& in, Range test_range = Range { 200000000000000LL, 400000000000000LL }) {
  std::vector<Hailstone> hailstones = parse_input(in);
  int64_t result = 0;
  for (int i = 0; i < hailstones.size(); i++) {
    const Hailstone& a = hailstones[i];
    for (int j = i + 1; j < hailstones.size(); j++) {
      const Hailstone& b = hailstones[j];
      Vec3 intersection = intersect_2d(a, b);

      if (intersection.z < 0) {
        // parallel, skip
        continue;
      }

      std::cout << "Hailstone A: " << a << std::endl;
      std::cout << "Hailstone B: " << b << std::endl;
      std::cout << "Hailstones intersection: " << intersection << std::endl;
      if (!test_range.includes(intersection.x) || !test_range.includes(intersection.y)) {
        // outside region, skip
        continue;
        std::cout << "Hailstones intersect outside test region" << std::endl;
      } else {
        bool a_future = (intersection.x - a.position.x) / a.velocity.x >= 0 && (intersection.y - a.position.y) / a.velocity.y >= 0;
        bool b_future = (intersection.x - b.position.x) / b.velocity.x >= 0 && (intersection.y - b.position.y) / b.velocity.y >= 0;
        if (a_future && b_future) {
          // future intersection
          std::cout << "Hailstones will intersect in the future" << std::endl;
          result++;
        } else {
          // past intersection
          std::cout << "Hailstones intersected in the past" << std::endl;
        }
      }
      std::cout << std::endl;
    }
  }
  return result;
}

int64_t part1(std::string in, Range test_range) {
  std::istringstream iss(in);
  return part1(iss, test_range);
}

}  // namespace day24
}  // namespace AOC
