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
namespace day13 {

using ranges::views::filter;
using ranges::views::transform;

typedef std::vector<std::string> Input;

std::vector<Input> parse_input(std::basic_istream<char>& in) {
  std::vector<Input> result;

  std::string line;
  while (std::getline(in, line)) {
    Input input;
    input.push_back(line);
    while (std::getline(in, line) && !line.empty()) {
      input.push_back(line);
    }
    result.push_back(input);
  }
  return result;
}

bool reflects_vertically_at(const Input& input, int col) {
  int width = input[0].size();
  for (int j = 0; j < col; j++) {
    int left = col - j - 1;
    int right = col + j;
    if (right >= width) break;

    for (int y = 0; y < input.size(); y++) {
      if (input[y][left] != input[y][right]) return false;
    }
  }
  return true;
}

int find_vertical_reflection_line(const Input& input) {
  int width = input[0].size();
  for (int i = 1; i < width; i++) {
    if (reflects_vertically_at(input, i)) return i;
  }
  return 0;
}

bool reflects_horizontally_at(const Input& input, int row) {
  int height = input.size();
  for (int j = 0; j < row; j++) {
    int top = row - j - 1;
    int bottom = row + j;
    if (bottom >= height) break;
    if (input[top] != input[bottom]) return false;
  }
  return true;
}

int find_horizontal_reflection_line(const Input& input) {
  int height = input.size();
  for (int i = 1; i < height; i++) {
    if (reflects_horizontally_at(input, i)) return i;
  }
  return 0;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<Input> inputs = parse_input(in);
  int result = 0;
  for (const Input& input : inputs) {
    int input_result = 0;
    input_result += find_vertical_reflection_line(input);
    input_result += 100 * find_horizontal_reflection_line(input);
    result += input_result;
  }
  return result;
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

}  // namespace day13
}  // namespace AOC
