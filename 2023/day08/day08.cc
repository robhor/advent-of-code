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

int64_t part2(std::basic_istream<char>& in) {
  return 0;
}

}  // namespace day08
}  // namespace AOC
