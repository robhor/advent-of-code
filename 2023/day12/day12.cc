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
#include "range/v3/view/all.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"

namespace AOC {
namespace day12 {

using ranges::views::filter;
using ranges::views::transform;

struct Input {
  std::string mask;
  std::vector<int> group_sizes;
};

std::ostream& operator<<(std::ostream& os, const Input& input) {
  os << "{";
  os << input.mask << " ";
  for (int i : input.group_sizes) {
    os << i << ",";
  }
  os << "}";
  return os;
}

std::vector<Input> parse_input(std::basic_istream<char>& in) {
  std::vector<Input> result;
  std::string line;
  while(std::getline(in, line)) {
    Input input;
    std::vector<absl::string_view> parts = absl::StrSplit(line, " ");
    input.mask = std::string(parts[0]);

    std::vector<absl::string_view> groups = absl::StrSplit(parts[1], ",");
    input.group_sizes = groups | transform([](const absl::string_view& g) { int i; assert(absl::SimpleAtoi(g, &i)); return i; }) | ranges::to<std::vector>();
    result.push_back(input);
  }
  return result;
}

bool can_place_group_at(int i, int size, absl::string_view mask) {
  // Can't overflow
  if (i + size > mask.size()) return false;

  // Can't end in a # as that would extend group
  if (i + size < mask.size() && mask[i + size] == '#') return false;

  // Can't be placed after the start of a group
  for (int j = 0; j < i; j++) {
    if (mask[j] == '#') return false;
  }
  
  // Can't contain .s
  for (int j = i; j < i + size; j++) {
    if (mask[j] == '.') return false;
  }

  return true;
}

int arrangements(absl::string_view mask, std::span<int const> group_sizes) {
  if (group_sizes.empty()) {
    for (int i = 0; i < mask.size(); i++) {
      if (mask[i] == '#') return 0;
    }
    return 1;
  }

  int group_size_sum = ranges::accumulate(group_sizes, 0, [](int a, int b) { return a + b; });
  int min_padding = group_sizes.size() - 1;
  int min_space_required = group_size_sum + min_padding;

  if (mask.size() < min_space_required) {
    return 0;
  }

  int first_group_size = group_sizes.front();
  int remaining_space_required = min_space_required - first_group_size;

  int result = 0;
  for (int i = 0; i < mask.size() - first_group_size - remaining_space_required + 1; i++) {
    if (!can_place_group_at(i, first_group_size, mask)) {
      continue;
    }
    result += arrangements(absl::ClippedSubstr(mask, i + first_group_size + 1), group_sizes.subspan(1));
  }

  return result;
}

int arrangements(const Input& input) {
  return arrangements(input.mask, std::span(input.group_sizes));
}


int64_t part1(std::basic_istream<char>& in) {
  std::vector<Input> inputs = parse_input(in);
  return ranges::accumulate(inputs | transform([](const auto& input) { return arrangements(input); }), 0, [](int a, int b) { return a+b;});
}

int64_t part2(std::basic_istream<char>& in) {
  return 0;
}

int64_t part1(std::string in) {
  std::istringstream iss(in);
  return part1(iss);
}

}  // namespace day12
}  // namespace AOC
