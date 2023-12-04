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
namespace day04 {

struct Scratchcard {
  std::vector<int> winning_numbers;
  std::vector<int> actual_numbers;

  auto operator<=>(const Scratchcard&) const = default;

  std::vector<int> matching_numbers() const {
    std::vector<int> result;
    for (int winning_number : winning_numbers) {
      for (int actual_number : actual_numbers) {
        if (winning_number == actual_number) {
          result.push_back(winning_number);
          break;
        }
      }
    }
    return result;
  }

  int num_matching_numbers() const {
    return this->matching_numbers().size();
  }
      
  int points() const {
    int num_matching_numbers = this->num_matching_numbers();
    if (num_matching_numbers == 0) {
      return 0;
    }

    return std::pow(2, num_matching_numbers - 1);
  }
};

void split_numbers(absl::string_view line, std::vector<int>* result) {
  std::vector<absl::string_view> numbers = absl::StrSplit(line, ' ', absl::SkipWhitespace());
  for (absl::string_view number : numbers) {
    int out;
    absl::SimpleAtoi(number, &out);
    result->push_back(out);
  }
}

Scratchcard parse_scratchcard(const std::string& line) {
  Scratchcard result;
  absl::string_view numbers_part =
      ((std::vector<absl::string_view>)absl::StrSplit(line,
                                                      absl::ByString(": ")))[1];
  std::vector<absl::string_view> winning_and_actual_numbers =
      absl::StrSplit(numbers_part, '|');

  auto to_int = [](absl::string_view& v) {
    int out;
    absl::SimpleAtoi(v, &out);
    return out;
  };

  split_numbers(winning_and_actual_numbers[0], &result.winning_numbers);
  split_numbers(winning_and_actual_numbers[1], &result.actual_numbers);
  return result;
}

std::vector<Scratchcard> parse_input(std::basic_istream<char>& in) {
  std::vector<Scratchcard> result;
  std::string line;
  while (std::getline(in, line)) {
    result.push_back(parse_scratchcard(line));
  }
  return result;
}

int part1(std::basic_istream<char>& in) {
  auto scratchcards = parse_input(in);
  int sum = 0;
  for (const auto& card : scratchcards) {
    sum += card.points();
  }
  return sum;
}

struct ScratchcardWithCount {
  const Scratchcard& scratchcard;
  int64_t count;
};

int part2(std::basic_istream<char>& in) {
  std::vector<Scratchcard> scratchcards = parse_input(in);
  std::vector<ScratchcardWithCount> scratchcards_pile;
  for (const Scratchcard& scratchcard : scratchcards) {
    scratchcards_pile.push_back({
      .scratchcard = scratchcard,
      .count = 1
    });
  }

  for (int i = 0; i < scratchcards_pile.size(); i++) {
    ScratchcardWithCount& card = scratchcards_pile[i];
    int won_copies = card.scratchcard.num_matching_numbers();
    for (int j = 0; i + 1 + j < scratchcards_pile.size() && j < won_copies; j++) {
      scratchcards_pile[i + 1 + j].count += card.count;
    }
  }

  int64_t sum = 0;
  int i = 1;
  for (const auto& card : scratchcards_pile) {
    sum += card.count;
  }
  return sum;
}

}  // namespace day04
}  // namespace AOC
