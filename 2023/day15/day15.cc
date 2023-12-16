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

struct Instruction {
  absl::string_view label;
  char command;
  int lens {0};
};

struct Lens {
  absl::string_view label;
  int lens;
};

struct Box {
  std::vector<Lens> lenses;
};

std::vector<Instruction> parse_input2(const std::string& in) {
  std::vector<absl::string_view> parts = absl::StrSplit(in, ",");
  return parts | transform([](const absl::string_view& part) {
    int op_idx = -1;
    for (int i = 1; i < part.size(); i++) {
      if (part[i] == '-' || part[i] == '=') {
        op_idx = i;
        break;
      }
    }
    char op = part[op_idx];
    absl::string_view label = part.substr(0, op_idx);
    int lens = 0;
    absl::SimpleAtoi(part.substr(op_idx + 1), &lens);
    return Instruction {label, op, lens};
  }) | ranges::to<std::vector>();
}

int64_t part2(std::basic_istream<char>& in) {
  std::string line;
  std::getline(in, line);
  std::vector<Instruction> instructions = parse_input2(line);
  std::array<Box, 256> boxes;

  for (const Instruction& instruction : instructions) {
    Box& box = boxes[hash(instruction.label)];
    std::vector<Lens>::iterator it = box.lenses.begin();
    while (it < box.lenses.end()) {
      if ((*it).label == instruction.label) {
        if (instruction.command == '=') {
          (*it).lens = instruction.lens;
        } else {
          box.lenses.erase(it);
        }
        break;
      }
      it++;
    }
    if (it == box.lenses.end()) {
      if (instruction.command == '=') {
        box.lenses.push_back(Lens { instruction.label, instruction.lens });
      }
    }
  }

  int64_t result = 0;
  for (int i = 0; i < boxes.size(); i++) {
    const Box& box = boxes[i];
    for (int j = 0; j < box.lenses.size(); j++) {
      int64_t focusing_power = 1;
      focusing_power *= (i + 1);
      focusing_power *= (j + 1);
      focusing_power *= box.lenses[j].lens;
      result += focusing_power;
    }
  }

  return result;
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
