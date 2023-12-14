#include "day14.cc"

#include <gtest/gtest.h>

#include <string>

namespace {

using namespace AOC::day14;

std::string example_input = R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....)";

std::string example_input_tilted_north = R"(OOOO.#.O..
OO..#....#
OO..O##..O
O..#.OO...
........#.
..#....#.#
..O..#.O.O
..O.......
#....###..
#....#....)";

std::vector<std::string> lines(const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        result.push_back(line);
    }
    return result;
}


TEST(day14, Part1) {
  EXPECT_EQ(tilt_north(lines(example_input)), lines(example_input_tilted_north));
  EXPECT_EQ(load(lines(example_input_tilted_north)), 136);
  EXPECT_EQ(part1(example_input), 136);
}

TEST(day14, Part2) {
  EXPECT_EQ(part2(example_input), 0);
}

}  // namespace