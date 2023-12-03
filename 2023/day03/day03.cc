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

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace AOC {
namespace day03 {

struct Point {
  int line;
  int pos;
};

struct NumberRange {
  int number;
  Point start;
  int length;
};

struct Symbol {
  char symbol;
  Point pos;
};

struct Gear {
  Point pos;
  std::array<NumberRange, 2> ranges;
};

std::vector<std::string> read_lines(std::basic_istream<char>& in) {
  std::vector<std::string> result;
  std::string line;
  while (std::getline(in, line)) {
    result.push_back(line);
  }
  return result;
}

bool is_digit(char d) { return d >= '0' && d <= '9'; }

std::vector<NumberRange> get_number_ranges(int line_num,
                                           const std::string& line) {
  std::vector<NumberRange> result;
  int start_idx = -1;
  for (int i = 0; i < line.length(); i++) {
    if (is_digit(line[i]) && start_idx == -1) {
      start_idx = i;
    } else if (!is_digit(line[i]) && start_idx != -1) {
      const int length = i - start_idx;
      const absl::string_view num_view =
          absl::string_view(line).substr(start_idx, length);
      int num;
      absl::SimpleAtoi(num_view, &num);
      NumberRange range{.start = {.line = line_num, .pos = start_idx},
                        .length = length,
                        .number = num};
      result.push_back(range);
      start_idx = -1;
    }
  }

  if (start_idx != -1) {
    const int length = line.length() - start_idx;
    const absl::string_view num_view =
        absl::string_view(line).substr(start_idx, length);
    int num;
    absl::SimpleAtoi(num_view, &num);
    NumberRange range{.start = {.line = line_num, .pos = start_idx},
                      .length = length,
                      .number = num};
    result.push_back(range);
  }

  return result;
}

std::vector<NumberRange> get_number_ranges(
    const std::vector<std::string>& lines) {
  std::vector<NumberRange> result;
  for (int line = 0; line < lines.size(); line++) {
    std::vector<NumberRange> ranges = get_number_ranges(line, lines[line]);
    result.insert(result.end(), ranges.begin(), ranges.end());
  }
  return result;
}

std::vector<Symbol> get_symbols(const std::vector<std::string>& lines) {
  std::vector<Symbol> result;
  for (int line_idx = 0; line_idx < lines.size(); line_idx++) {
    const std::string& line = lines[line_idx];
    for (int i = 0; i < line.length(); i++) {
      if (!is_digit(line[i]) && line[i] != '.') {
        result.push_back(
          Symbol {
            .symbol = line[i],
            .pos = {.line = line_idx, .pos = i}
          });
      }
    }
  }
  return result;
}

bool is_neighboring_point(const NumberRange& range, const Point& point) {
  if (std::abs(point.line - range.start.line) > 1) return false;
  return point.pos >= range.start.pos - 1 && point.pos <= range.start.pos + range.length;
}

bool has_neighboring_symbol(const NumberRange& range, const std::vector<Symbol>& symbols) {
  for (const Symbol& symbol : symbols) {
    if (is_neighboring_point(range, symbol.pos)) {
      return true;
    }
  }
  return false;
}

std::vector<NumberRange> filter_ranges(const std::vector<NumberRange>& number_ranges,
                                       const std::vector<Symbol>& symbols) {
  std::vector<NumberRange> result;
  for (const NumberRange& number_range : number_ranges) {
    if (has_neighboring_symbol(number_range, symbols)) {
      result.push_back(number_range);
    }
  }
  return result;
}

int part1(std::basic_istream<char>& in) {
  const std::vector<std::string> lines = read_lines(in);
  const std::vector<NumberRange> number_ranges = get_number_ranges(lines);
  const std::vector<Symbol> symbols = get_symbols(lines);
  const std::vector<NumberRange> part_numbers = filter_ranges(number_ranges, symbols);

  int result = 0;
  for (const auto& number_range : part_numbers) {
    result += number_range.number;
  }

  return result;
}

std::optional<Gear> get_gear(const std::vector<NumberRange>& number_ranges, const Symbol& symbol) {
  if (symbol.symbol != '*') return std::nullopt;
  Gear gear { .pos = symbol.pos };
  int found_neighbors = 0;
  for (const NumberRange& range : number_ranges) {
    if (is_neighboring_point(range, symbol.pos)) {
      if (found_neighbors == 2) {
        return std::nullopt;
      }
      gear.ranges[found_neighbors++] = range;
    }
  }

  if (found_neighbors != 2) {
    return std::nullopt;
  }
  return gear;
}

std::vector<Gear> get_gears(const std::vector<NumberRange>& number_ranges, const std::vector<Symbol>& symbols) {
  std::vector<Gear> result;
  for (const Symbol& symbol : symbols) {
    std::optional<Gear> gear = get_gear(number_ranges, symbol);
    if (gear.has_value()) {
      result.push_back(gear.value());
    }
  }
  return result;
}

int part2(std::basic_istream<char>& in) {
  const std::vector<std::string> lines = read_lines(in);
  const std::vector<NumberRange> number_ranges = get_number_ranges(lines);
  const std::vector<Symbol> symbols = get_symbols(lines);
  const std::vector<NumberRange> part_numbers = filter_ranges(number_ranges, symbols);
  const std::vector<Gear> gears = get_gears(part_numbers, symbols);

  int result = 0;
  for (const Gear& gear : gears) {
    result += gear.ranges[0].number * gear.ranges[1].number;
  }
  return result;
}

}  // namespace day03
}  // namespace AOC
