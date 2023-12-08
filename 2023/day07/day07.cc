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

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace AOC {
namespace day07 {

const int HAND_SIZE = 5;
const int JOKER = 11;
typedef std::array<int, HAND_SIZE> Hand;

enum HandKind {
  HIGH_CARD = 0,
  ONE_PAIR = 1,
  TWO_PAIR = 22,
  THREE_OF_A_KIND = 31,
  FULL_HOUSE = 32,
  FOUR_OF_A_KIND = 41,
  FIVE_OF_A_KIND = 51
};

struct HandBid {
  Hand hand;
  int bid;
};

HandKind kind_of(Hand hand, bool with_joker = false) {
  std::map<int, int> card_occurences;
  for (char card : hand) {
    card_occurences[card]++;
  }
  if (with_joker && card_occurences.contains(JOKER)) {
    int jokers = card_occurences[JOKER];
    if (jokers == 5) return FIVE_OF_A_KIND;
    card_occurences.erase(JOKER);
    auto max_value_card = std::max_element(card_occurences.begin(), card_occurences.end(), [](std::pair<int, int> a, std::pair<int, int> b){
      if (a.second == b.second) return a.first < b.first;
      return a.second < b.second;
    });
    max_value_card->second += jokers;
  }
  std::vector<int> card_counts;
  for (const auto& elem : card_occurences) {
    card_counts.push_back(elem.second);
  }
  std::sort(card_counts.begin(), card_counts.end());

  if (card_counts.size() == 1) {
    return FIVE_OF_A_KIND;
  }
  if (card_counts.size() == 2 && card_counts[0] == 1) {
    return FOUR_OF_A_KIND;
  }
  if (card_counts.size() == 2 && card_counts[0] == 2) {
    return FULL_HOUSE;
  }
  if (card_counts[card_counts.size() - 1] == 3) {
    return THREE_OF_A_KIND;
  }
  if (card_counts[card_counts.size() - 1] == 2 && card_counts[card_counts.size() - 2] == 2) {
    return TWO_PAIR;
  }
  if (card_counts[card_counts.size() - 1] == 2) {
    return ONE_PAIR;
  }
  assert(card_counts.size() == HAND_SIZE);
  return HIGH_CARD;
}

// Returns true if [first] is stronger than [second]
bool is_stronger_than(const Hand& first, const Hand& second, bool with_joker = false) {
  if (kind_of(first, with_joker) > kind_of(second, with_joker)) return true;
  if (kind_of(first, with_joker) < kind_of(second, with_joker)) return false;

  auto card_value = [with_joker](int card) {
    if (with_joker && card == JOKER) return 1;
    return card;
  };
  
  for (int i = 0; i < HAND_SIZE; i++) {
    if (first[i] == second[i]) continue;
    return card_value(first[i]) > card_value(second[i]);
  }

  return false;
}

constexpr int strengh_of_card(char card) {
  switch (card) {
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'T': return 10;
    case 'J': return 11;
    case 'Q': return 12;
    case 'K': return 13;
    case 'A': return 14;
  }
  assert(false && "Invalid card");
}
static_assert(strengh_of_card('J') == JOKER);

Hand parse_hand(absl::string_view hand) {
  return Hand {
    strengh_of_card(hand[0]),
    strengh_of_card(hand[1]),
    strengh_of_card(hand[2]),
    strengh_of_card(hand[3]),
    strengh_of_card(hand[4]),
  };
}

std::vector<HandBid> parse_input(std::basic_istream<char>& in) {
  std::vector<HandBid> result;
  std::string line;
  while (std::getline(in, line)) {
    Hand hand = parse_hand(line);
    int bid;
    absl::SimpleAtoi(absl::string_view(line).substr(HAND_SIZE + 1), &bid);
    result.push_back(HandBid { hand, bid });
  }
  return result;
}

void rank_hands(std::vector<HandBid>& hand_bids, bool with_joker = false) {
  std::sort(hand_bids.begin(), hand_bids.end(), [with_joker](const HandBid& a, const HandBid& b) { return is_stronger_than(b.hand, a.hand, with_joker); });
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<HandBid> hands = parse_input(in);
  rank_hands(hands);

  int solution = 0;
  for (int i = 0; i < hands.size(); i++) {
    int rank = i + 1;
    int prev = solution;
    solution += hands[i].bid * rank;
    assert(solution > prev);
  }
  return solution;
}

HandKind kind_with_joker(Hand hand) {
  return kind_of(hand, /* with_joker= */ true);
}

int64_t part2(std::basic_istream<char>& in) {
  std::vector<HandBid> hands = parse_input(in);
  rank_hands(hands, /* with_joker= */ true);

  int solution = 0;
  for (int i = 0; i < hands.size(); i++) {
    int rank = i + 1;
    int prev = solution;
    solution += hands[i].bid * rank;
    assert(solution > prev);
  }
  return solution;
}

}  // namespace day07
}  // namespace AOC
