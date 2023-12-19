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
namespace day19 {

using ranges::views::filter;
using ranges::views::transform;

const int ACCEPT = -1;
const int REJECT = -2;
const int MAX_CATEGORY_VALUE = 4000;

struct Condition {
  char category;
  char op;
  int operand;
};

std::ostream& operator<<(std::ostream& os, const Condition& condition) {
  os << condition.category;
  os << condition.op;
  os << condition.operand;
  return os;
}

struct Rule {
  Condition condition;
  std::string destination;
};

std::ostream& operator<<(std::ostream& os, const Rule& rule) {
  if (rule.condition.op != '_') {
    os << rule.condition;
    os << ":";
  }
  os << rule.destination;
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
  for (const auto& el : vec) {
    os << el << ",";
  }
  return os;
}

struct Workflow {
  std::string name;
  std::vector<Rule> rules;
};

std::ostream& operator<<(std::ostream& os, const Workflow& workflow) {
  os << workflow.name;
  os << "{";
  os << workflow.rules;
  os << "}";
  return os;
}

struct Part {
  int x;
  int m;
  int a;
  int s;
};

std::ostream& operator<<(std::ostream& os, const Part& part) {
  os << "{";
  os << "x=" << part.x << ",";
  os << "m=" << part.m << ",";
  os << "a=" << part.a << ",";
  os << "s=" << part.s << "";
  os << "}";
  return os;
}

struct Input {
  std::vector<Workflow> workflows;
  std::vector<Part> parts;
};

Rule parse_rule(absl::string_view line) {
  Rule rule;
  rule.condition.category = '_';
  rule.condition.op = '_';
  rule.condition.operand = 0;
  if (line == "R") {
    rule.destination = "R";
  } else if (line == "A") {
    rule.condition.op = '_';
    rule.destination = "A";
  } else {
    int colon_idx = line.find(':');
    if (colon_idx == -1) {
      rule.destination = std::string(line.substr(colon_idx + 1));
    } else {
      rule.destination = std::string(line.substr(colon_idx + 1));
      absl::string_view condition = line.substr(0, colon_idx);
      rule.condition.category = condition[0];
      rule.condition.op = condition[1];
      assert(absl::SimpleAtoi(condition.substr(2), &rule.condition.operand));
    }
  }
  return rule;
}

Workflow parse_workflow(absl::string_view line) {
  Workflow workflow;
  int brace_idx = line.find('{');
  workflow.name = line.substr(0, brace_idx);

  line = line.substr(brace_idx + 1);
  line = line.substr(0, line.size() - 1);
  std::vector<absl::string_view> rules = absl::StrSplit(line, ',');
  for (absl::string_view rulev : rules) {
    Rule rule = parse_rule(rulev);
    workflow.rules.push_back(rule);
  }
  return workflow;
}

Input parse_input(std::basic_istream<char>& in) {
  Input result;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) break;
    result.workflows.push_back(parse_workflow(line));
  }

  while (std::getline(in, line)) {
    Part part;
    absl::string_view linev(line);
    linev = linev.substr(1);
    linev = linev.substr(0, linev.size() - 1);
    std::vector<absl::string_view> props = absl::StrSplit(linev, ',');
    assert(absl::SimpleAtoi(props[0].substr(2), &part.x));
    assert(absl::SimpleAtoi(props[1].substr(2), &part.m));
    assert(absl::SimpleAtoi(props[2].substr(2), &part.a));
    assert(absl::SimpleAtoi(props[3].substr(2), &part.s));
    result.parts.push_back(part);
  }
  return result;
}

struct IndexedRule {
  Condition condition;
  int destination;
};

std::ostream& operator<<(std::ostream& os, const IndexedRule& rule) {
  os << rule.condition;
  os << ":";
  os << rule.destination;
  return os;
}

struct IndexedWorkflows {
  std::vector<std::vector<IndexedRule>> workflows;
  int in_workflow;
};

typedef std::vector<IndexedRule> IndexedWorkflow;

IndexedWorkflows index_workflows(const std::vector<Workflow>& workflows) {
  IndexedWorkflows result;
  for (const Workflow& workflow : workflows) {
    std::vector<IndexedRule> indexed_workflow;
    for (const Rule& rule : workflow.rules) {
      IndexedRule indexed_rule;
      indexed_rule.condition = rule.condition;

      if (rule.destination == "A") {
        indexed_rule.destination = ACCEPT;
      } else if (rule.destination == "R") {
        indexed_rule.destination = REJECT;
      } else {
        auto workflow_it = std::find_if(
            workflows.begin(), workflows.end(),
            [&rule](const Workflow& w) { return w.name == rule.destination; });
        assert(workflow_it != workflows.end());
        indexed_rule.destination =
            std::distance(workflows.begin(), workflow_it);
        assert(indexed_rule.destination >= 0);
      }
      indexed_workflow.push_back(indexed_rule);
    }
    if (workflow.name == "in") {
      result.in_workflow = result.workflows.size();
    }
    result.workflows.push_back(indexed_workflow);
  }
  return result;
}

bool matches(Condition cond, const Part part) {
  if (cond.op == '_') {
    return true;
  }
  int operand = cond.operand;
  if (cond.op == '<') {
    switch (cond.category) {
      case 'x':
        return part.x < operand;
      case 'm':
        return part.m < operand;
      case 'a':
        return part.a < operand;
      case 's':
        return part.s < operand;
      default:
        assert(false);
    }
  }

  if (cond.op == '>') {
    switch (cond.category) {
      case 'x':
        return part.x > operand;
      case 'm':
        return part.m > operand;
      case 'a':
        return part.a > operand;
      case 's':
        return part.s > operand;
      default:
        assert(false);
    }
  }

  assert(false);
}

int process_part(const IndexedWorkflows& iw, const IndexedWorkflow* wf,
                 const Part part) {
  for (const IndexedRule& rule : *wf) {
    if (matches(rule.condition, part)) {
      int next_workflow = rule.destination;
      if (next_workflow < 0) return next_workflow;
      return process_part(iw, &iw.workflows[next_workflow], part);
    }
  }
  assert(false);
}

int process_part(const IndexedWorkflows& iw, const Part part) {
  const IndexedWorkflow* workflow = &iw.workflows[iw.in_workflow];
  return process_part(iw, workflow, part);
}

int64_t rating_sum(const Part& part) {
  return part.x + part.m + part.a + part.s;
}

int64_t part1(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  IndexedWorkflows workflows = index_workflows(input.workflows);

  int64_t sum = 0;
  for (const Part& part : input.parts) {
    int workflow = process_part(workflows, part);
    if (workflow == ACCEPT) {
      sum += rating_sum(part);
    }
  }

  return sum;
}

struct Range {
  int start = 1;
  int length = MAX_CATEGORY_VALUE;

  Range() = default;
  Range(int start, int length) : start(start), length(length) {};
  Range(const Range &other) {
    start = other.start;
    length = other.length;
  }

  static Range ZERO;

  int end() const { return start + length - 1; }

  int64_t count() const {
    return length;
  }

  Range range_less_than(int limit) {
    if (end() < limit) return *this;
    if (limit < start) return Range::ZERO;
    return Range(start, limit - start);
  }

  Range range_greater_than(int limit) {
    if (start > limit) return *this;
    if (limit > end()) return Range::ZERO;
    return Range(limit + 1, end() - limit);
  }
};

Range Range::ZERO = Range { 0, 0 };

struct PartRange {
  Range x;
  Range m;
  Range a;
  Range s;

  PartRange() = default;
  PartRange(Range x, Range m, Range a, Range s): x(x), m(m), a(a), s(s) {}
  PartRange(const PartRange &other) {
    x = other.x;
    m = other.m;
    a = other.a;
    s = other.s;
  }

  int64_t count() const {
    return x.count() * m.count() * a.count() * s.count();
  }

  PartRange subset_matching_condition(const Condition& condition) {
    if (condition.op == '_') return *this;
    if (condition.op == '<') {
      switch (condition.category) {
      case 'x': return PartRange { x.range_less_than(condition.operand), m, a, s };
      case 'm': return PartRange { x, m.range_less_than(condition.operand), a, s };
      case 'a': return PartRange { x, m, a.range_less_than(condition.operand), s };
      case 's': return PartRange { x, m, a, s.range_less_than(condition.operand) };
      }
    }
    if (condition.op == '>') {
      switch (condition.category) {
      case 'x': return PartRange { x.range_greater_than(condition.operand), m, a, s };
      case 'm': return PartRange { x, m.range_greater_than(condition.operand), a, s };
      case 'a': return PartRange { x, m, a.range_greater_than(condition.operand), s };
      case 's': return PartRange { x, m, a, s.range_greater_than(condition.operand) };
      }
    }
  }

  PartRange subset_not_matching_condition(const Condition& condition) {
    if (condition.op == '_') return PartRange { Range::ZERO, Range::ZERO, Range::ZERO, Range::ZERO };
    if (condition.op == '<') {
      switch (condition.category) {
      case 'x': return PartRange { x.range_greater_than(condition.operand - 1), m, a, s };
      case 'm': return PartRange { x, m.range_greater_than(condition.operand - 1), a, s };
      case 'a': return PartRange { x, m, a.range_greater_than(condition.operand - 1), s };
      case 's': return PartRange { x, m, a, s.range_greater_than(condition.operand - 1) };
      }
    }
    if (condition.op == '>') {
      switch (condition.category) {
      case 'x': return PartRange { x.range_less_than(condition.operand + 1), m, a, s };
      case 'm': return PartRange { x, m.range_less_than(condition.operand + 1), a, s };
      case 'a': return PartRange { x, m, a.range_less_than(condition.operand + 1), s };
      case 's': return PartRange { x, m, a, s.range_less_than(condition.operand + 1) };
      }
    }
  }
};

int64_t accepted_combinations(const IndexedWorkflows& wfs, int wfi, PartRange range) {
  if (range.count() == 0) return 0;

  const IndexedWorkflow* wf = &wfs.workflows[wfi];
  PartRange remaining_range = range;
  int64_t result = 0;
  for (const IndexedRule& rule : *wf) {
    PartRange subrange = remaining_range.subset_matching_condition(rule.condition);
    remaining_range = remaining_range.subset_not_matching_condition(rule.condition);

    if (rule.destination == REJECT) {
      continue;
    } else if (rule.destination == ACCEPT) {
      result += subrange.count();
    } else {
      result += accepted_combinations(wfs, rule.destination, subrange);
    }
  }

  return result;
}

int64_t accepted_combinations(const IndexedWorkflows& wfs) {
  PartRange part_range;
  return accepted_combinations(wfs, wfs.in_workflow, part_range);
}

int64_t part2(std::basic_istream<char>& in) {
  Input input = parse_input(in);
  IndexedWorkflows workflows = index_workflows(input.workflows);
  return accepted_combinations(workflows);
}

int64_t part1(std::string in) {
  std::istringstream iss(in);
  return part1(iss);
}

int64_t part2(std::string in) {
  std::istringstream iss(in);
  return part2(iss);
}

}  // namespace day19
}  // namespace AOC
