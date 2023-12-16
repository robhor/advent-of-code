#include "day15.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day15;

TEST(day15, Hash) {
  EXPECT_EQ(hash("HASH"), 52);
}

TEST(day15, Part1) {
  EXPECT_EQ(part1("rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"), 1320);
}

TEST(day15, Part2) {
  EXPECT_EQ(part2("rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"), 145);
}

}  // namespace