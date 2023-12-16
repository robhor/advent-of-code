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
namespace day15 {

using ranges::views::filter;
using ranges::views::transform;

std::vector<std::string> parse_input(std::basic_istream<char>& in) {
  std::string line;
  std::getline(in, line);
  return absl::StrSplit(line, ",");
}

int hash(absl::string_view str) {
  int result = 0;
  for (char c : str) {
    result += c;
    result *= 17;
    result = result % 256;
  }
  return result;
}

int64_t part1(std::basic_istream<char>& in) {
  int result = 0;
  for (const std::string& part : parse_input(in)) {
    result += hash(part);
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

}  // namespace day15
}  // namespace AOC
