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
namespace day08 {

const std::string START = "AAA";
const std::string END = "ZZZ";
const char LEFT = 'L';
const char RIGHT = 'R';

struct Node {
  std::string name;
  std::string left;
  std::string right;
};

struct Input {
  std::string instructions;
  std::vector<Node> nodes;
};

Input parse_input(std::basic_istream<char>& in) {
  Input result;
  std::string line;
  std::getline(in, line);
  result.instructions = line;

  std::getline(in, line); // empty line
 
  while(std::getline(in, line)) {
    absl::string_view line_view(line);
    std::string name = std::string(line_view.substr(0, 3));
    std::string left = std::string(line_view.substr(7, 3));
    std::string right = std::string(line_view.substr(12, 3));
    result.nodes.push_back(Node { name, left, right });
  }

  return result;
}

class Part1State {
  public:
  Part1State(Input input) {
    this->instructions = input.instructions;
    for (const Node& node : input.nodes) {
      this->nodes[node.name] = node;
    }
    current_state = &this->nodes[START];
  }

  Part1State(const Part1State& other) noexcept {
    instructions = other.instructions;
    current_instruction = other.current_instruction;
    nodes = other.nodes;
    current_state = &nodes[other.current_state->name];
  }

  Node* advance() {
    char instruction = instructions[current_instruction];
    current_instruction = (current_instruction + 1) % instructions.size();

    if (instruction == LEFT) {
      current_state = &nodes[current_state->left];
    } else {
      current_state = &nodes[current_state->right];
    }

    return current_state;
  }

  std::string instructions;
  int current_instruction { 0 };
  Node* current_state;
  std::unordered_map<std::string, Node> nodes;
};

int64_t part1(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  Part1State state(input);

  int turns = 1;
  while (state.advance()->name != END) {
    turns++;
  }
  return turns;
}

class Part2State {
  public:
  Part2State(Input input) {
    for (const Node& node : input.nodes) {
      if (node.name[2] == 'A') {
        Part1State ghost_state(input);
        ghost_state.current_state = &ghost_state.nodes[node.name];
        ghost_states.push_back(ghost_state);
      }
    }
  }

  void advance() {
    for (Part1State& state : ghost_states) {
      Node* node = state.advance();
    }
  }

  std::vector<Part1State> ghost_states;
};

int64_t part2(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  Part2State state(input);

  int turn = 0;
  std::vector<int> first_z = std::vector<int>(state.ghost_states.size(), 0);
  int zs = 0;
  while (zs < first_z.size()) {
    turn++;
    state.advance();

    for (int i = 0; i < state.ghost_states.size(); i++) {
      Part1State& ghost = state.ghost_states[i];
      if (first_z[i] == 0 && ghost.current_state->name[2] == 'Z') {
        first_z[i] = turn;
        zs++;
      }
    }
  }

  int64_t lcm = 1;
  for (int z : first_z) {
    lcm = std::lcm(lcm, z);
  }
  return lcm;
}

}  // namespace day08
}  // namespace AOC
