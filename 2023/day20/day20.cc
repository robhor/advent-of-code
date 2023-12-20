#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <queue>
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
#include "range/v3/view/split.hpp"
#include "range/v3/view/transform.hpp"

namespace AOC {
namespace day20 {

using ranges::views::filter;
using ranges::views::transform;

char MODULE_TYPE_BROADCASTER = 'b';
char MODULE_TYPE_FLIP_FLOP = '%';
char MODULE_TYPE_CONJUNCTION = '&';
const std::string MODULE_NAME_BUTTON = "button";
const std::string MODULE_NAME_BROADCASTER = "broadcaster";
const int PULSE_LOW = 0;
const int PULSE_HIGH = 1;

struct Module {
  char type;
  std::string name;
  std::vector<std::string> outputs;
};

std::vector<Module> parse_input(std::basic_istream<char>& in) {
  std::vector<Module> modules;
  std::string line;
  while (std::getline(in, line)) {
    Module module;
    std::vector<absl::string_view> parts = absl::StrSplit(line, " -> ");
    module.type = parts[0][0];
    if (module.type == 'b') {
      module.name = std::string(parts[0]);
    } else {
      module.name = std::string(parts[0].substr(1));
    }
    module.outputs = absl::StrSplit(parts[1], ", ");
    modules.push_back(module);
  }
  return modules;
}

struct IndexedModules {
  std::vector<Module> modules;
  std::vector<int64_t> inputs;
  std::unordered_map<std::string, int> name_to_index;
};

IndexedModules index_modules(std::vector<Module>&& modules) {
  IndexedModules result;
  result.modules = modules;
  result.inputs.reserve(result.modules.size());

  for (int i = 0; i < result.modules.size(); i++) {
    result.inputs.push_back(0);
    const Module& module = result.modules[i];
    result.name_to_index[module.name] = i;
  }

  for (int i = 0; i < result.modules.size(); i++) {
    const Module& module = result.modules[i];

    int64_t mask = 1LL << i;
    for (const std::string& output : module.outputs) {
      result.inputs[result.name_to_index[output]] |= mask;
    }
  }
  return result;
}

struct Pulse {
  int type = PULSE_LOW;
  int src;
  int dst;
};

struct PulseCount {
  int64_t low_pulses = 0;
  int64_t high_pulses = 0;

  PulseCount operator+(const PulseCount& other) {
    return PulseCount{.low_pulses = low_pulses + other.low_pulses,
                      .high_pulses = high_pulses + other.high_pulses};
  }

  void operator+=(const PulseCount& other) {
    low_pulses += other.low_pulses;
    high_pulses += other.high_pulses;
  }
};

void update_destination_state(IndexedModules& modules,
                              std::vector<int64_t>& states, Pulse pulse) {
  const int module_idx = pulse.dst;
  const Module& module = modules.modules[module_idx];

  if (module.type == MODULE_TYPE_FLIP_FLOP) {
    if (pulse.type == PULSE_HIGH) {
    // Flip-Flops do nothing on a high pulse
      return;
    }

    // Flip the stored bit
    int64_t mask = (1LL << module_idx);
    int64_t& state = states[module_idx];
    bool was_on = (state & mask) > 0LL;
    bool is_on = !was_on;
    if (is_on) {
      state |= mask;
    } else {
      state &= ~mask;
    }
  } else if (module.type == MODULE_TYPE_CONJUNCTION) {
    // update incoming input state
    int64_t& state = states[module_idx];
    int64_t input_mask = (1LL << pulse.src);
    if (pulse.type == PULSE_HIGH) {
      state |= input_mask;
    } else {
      state &= ~input_mask;
    }
  }
}

// Process a pulse: Update state, and return downstream pulses
std::vector<Pulse> process_pulse(IndexedModules& modules,
                                 std::vector<int64_t>& states, Pulse pulse) {
  const int module_idx = pulse.dst;
  const Module& module = modules.modules[module_idx];

  // std::cout << modules.modules[pulse.src].name << " -"
  //           << ((pulse.type == PULSE_LOW) ? "low" : "high") << "-> "
  //           << module.name << std::endl;

  update_destination_state(modules, states, pulse);

  if (module.type == MODULE_TYPE_BROADCASTER) {
    std::vector<Pulse> new_pulses;
    for (const auto& out : module.outputs) {
      int out_idx = modules.name_to_index[out];
      Pulse new_pulse =
          Pulse{.src = module_idx, .dst = out_idx, .type = pulse.type};
      // update_destination_state(modules, states, new_pulse);
      new_pulses.push_back(new_pulse);
    }
    return new_pulses;
  } else if (module.type == MODULE_TYPE_FLIP_FLOP) {
    if (pulse.type == PULSE_HIGH) {
      // Flip-Flops do nothing on a high pulse
      return {};
    } else {
      // Check current state
      int64_t mask = (1LL << module_idx);
      int64_t& state = states[module_idx];
      bool is_on = (state & mask) > 0LL;
      int new_pulse_type = is_on ? PULSE_HIGH : PULSE_LOW;
      std::vector<Pulse> new_pulses;
      for (const auto& out : module.outputs) {
        int out_idx = modules.name_to_index[out];
        Pulse new_pulse =
            Pulse{.src = module_idx, .dst = out_idx, .type = new_pulse_type};
        // update_destination_state(modules, states, new_pulse);
        new_pulses.push_back(new_pulse);
      }
      return new_pulses;
    }
  } else if (module.type == MODULE_TYPE_CONJUNCTION) {
    // check all inputs
    int64_t& state = states[module_idx];
    int64_t all_inputs_mask = modules.inputs[module_idx];
    bool all_high = (state & all_inputs_mask) == all_inputs_mask;
    int new_pulse_type = all_high ? PULSE_LOW : PULSE_HIGH;

    std::vector<Pulse> new_pulses;
    for (const auto& out : module.outputs) {
      int out_idx = modules.name_to_index[out];
      Pulse new_pulse =
          Pulse{.src = module_idx, .dst = out_idx, .type = new_pulse_type};
      // update_destination_state(modules, states, new_pulse);
      new_pulses.push_back(new_pulse);
    }
    return new_pulses;
  }
  assert(false && "Unknown module type");
}

// Returns the number of low & high pulses triggered downstream
PulseCount push_button(IndexedModules& modules, std::vector<int64_t>& states) {
  PulseCount result;
  std::queue<Pulse> queue;
  Pulse starting_pulse =
      Pulse{.src = modules.name_to_index[MODULE_NAME_BUTTON],
            .dst = modules.name_to_index[MODULE_NAME_BROADCASTER],
            .type = PULSE_LOW};
  queue.push(starting_pulse);

  while (!queue.empty()) {
    const Pulse pulse = queue.front();
    queue.pop();

    if (pulse.type == PULSE_LOW) {
      result.low_pulses++;
    } else {
      result.high_pulses++;
    }

    std::vector<Pulse> downstream_pulses = process_pulse(modules, states, pulse);
    for (const auto& p : downstream_pulses) {
      queue.push(p);
    }
  }

  return result;
}

int64_t part1(std::basic_istream<char>& in) {
  std::vector<Module> modules = parse_input(in);
  modules.push_back(Module{.name = MODULE_NAME_BUTTON,
                           .outputs = {MODULE_NAME_BROADCASTER},
                           .type = 'b'});
  modules.insert(modules.begin(),
                 {Module{.name = "(unknown)", .outputs = {}, .type = 'b'}});
  assert(modules.size() < 64);
  std::vector<int64_t> state = std::vector<int64_t>(modules.size(), 0LL);
  IndexedModules imodules = index_modules(std::move(modules));

  PulseCount result;
  int button_presses = 1000;
  for (int i = 0; i < button_presses; i++) {
    result += push_button(imodules, state);
    // printf("------------------\n");
  }
  printf("low: %lld\nhigh: %lld\n", result.low_pulses, result.high_pulses);
  return result.low_pulses * result.high_pulses;
}

int64_t part2(std::basic_istream<char>& in) { return 0; }

int64_t part1(std::string in) {
  std::istringstream iss(in);
  return part1(iss);
}

int64_t part2(std::string in) {
  std::istringstream iss(in);
  return part2(iss);
}

}  // namespace day20
}  // namespace AOC
