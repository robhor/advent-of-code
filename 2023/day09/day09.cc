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

namespace AOC {
namespace day09 {

typedef std::vector<int> History;
typedef std::vector<History> Histories;

Histories parse_input(std::basic_istream<char>& in) {
  Histories result;
  std::string line;

  while(std::getline(in, line)) {
    History history;
    std::vector<absl::string_view> nums = absl::StrSplit(absl::string_view(line), ' ');
    for (absl::string_view num : nums) {
      int n;
      bool success = absl::SimpleAtoi(num, &n);
      assert(success);
      history.push_back(n);
    }
    result.push_back(history);
  }

  return result;
}

History differences(const History& history) {
  History result;
  for (int i = 0; i < history.size() - 1; i++) {
    result.push_back(history[i+1] - history[i]);
  }
  return result;
}

int extrapolate(const History& history) {
  if (std::all_of(history.begin(), history.end(), [](int n){return n == 0;})) {
    return 0;
  }
  int increment = extrapolate(differences(history));
  return history.back() + increment;
}

int extrapolate_front(const History& history) {
  if (std::all_of(history.begin(), history.end(), [](int n){return n == 0;})) {
    return 0;
  }
  int decrement = extrapolate_front(differences(history));
  return history.front() - decrement;
}

int64_t part1(std::basic_istream<char>& in) {
  Histories input = parse_input(in);
  int sum = 0;
  for (const History& history : input) {
    sum += extrapolate(history);
  }
  return sum;
}

int64_t part2(std::basic_istream<char>& in) {
  Histories input = parse_input(in);
  int sum = 0;
  for (const History& history : input) {
    sum += extrapolate_front(history);
  }
  return sum;
}

}  // namespace day09
}  // namespace AOC
