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

#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/strings/numbers.h"

namespace AOC {
namespace day02 {

struct GameRound {
    int red {0};
    int green {0};
    int blue {0};

    auto operator<=>(const GameRound&) const = default;
};

struct Game {
  int id;
  std::vector<GameRound> rounds;

  auto operator<=>(const Game&) const = default;
};

std::ostream& operator<<(std::ostream& os, const GameRound& round) {
  os << "{";
  os << round.red << " red, ";
  os << round.green << " green, ";
  os << round.blue << " blue";
  os << "}";
  return os;
}
template <typename V>
std::ostream& operator<<(std::ostream& os, const std::vector<V>& vec) {
  os << "{";
  for (const auto& el : vec) {
    os << el << ",";
  }
  os << "}";
  return os;
}
std::ostream& operator<<(std::ostream& os, const Game& game) {
  return os << "id: " << game.id << ", rounds: " << game.rounds << std::endl;
}

std::vector<GameRound> parse_game_rounds(absl::string_view line) {
    std::vector<GameRound> result;
    std::vector<absl::string_view> round_lines = absl::StrSplit(line, "; ");
    for (const auto& round_line : round_lines) {
        GameRound round;
        std::vector<absl::string_view> colors = absl::StrSplit(round_line, ", ");
        for (const auto& num_color : colors) {
            std::vector<absl::string_view> color_parts = absl::StrSplit(num_color, " ");
            int num;
            absl::SimpleAtoi(color_parts[0], &num);
            const absl::string_view& color = color_parts[1];
            if (color == "red") {
                round.red = num;
            } else if (color == "green") {
                round.green = num;
            } else if (color == "blue") {
                round.blue = num;
            } else {
                assert(false && "Unknown color");
            }
        }
        result.push_back(round);
    }

    return result;
}

std::vector<Game> parse_input(std::basic_istream<char>& in) {
  std::vector<Game> result;
  std::string line;
  while (std::getline(in, line)) {
    std::vector<absl::string_view> parts = absl::StrSplit(line, ": ");
    int id;
    auto id_view = parts[0].substr(5);
    absl::SimpleAtoi(id_view, &id);
    std::vector<GameRound> game_rounds = parse_game_rounds(parts[1]);
    result.push_back(Game {
      .id = id,
      .rounds = game_rounds
    });
  }
  return result;
}

bool is_valid(const GameRound& round, const GameRound& limit) {
  return round.red <= limit.red && round.green <= limit.green && round.blue <= limit.blue;
}

bool is_valid(const Game& game, const GameRound& limit) {
  for (const GameRound& round : game.rounds) {
    if (!is_valid(round, limit)) {
      return false;
    }
  }

  return true;
}

GameRound min_limit(const Game& game) {
  GameRound min;
  for (const GameRound& round : game.rounds) {
    min.red = std::max(min.red, round.red);
    min.green = std::max(min.green, round.green);
    min.blue = std::max(min.blue, round.blue);
  }
  return min;
}

int part1(std::basic_istream<char>& in) {
  GameRound limit = {
    .red = 12,
    .green = 13,
    .blue = 14
  };

  std::vector<Game> games = parse_input(in);
  int valid_games_id_sum = 0;

  for (const Game& game : games) {
    if (is_valid(game, limit)) {
      valid_games_id_sum += game.id;
    }
  }

  return valid_games_id_sum;
}

int power(const GameRound& round) {
  return round.red * round.green * round.blue;
}

int part2(std::basic_istream<char>& in) {
  std::vector<Game> games = parse_input(in);
  int sum = 0;
  for (const Game& game : games) {
    GameRound min = min_limit(game);
    sum += power(min);
  }
  return sum;
}

}  // namespace day02
}  // namespace AOC
