#include "day09.cc"

#include <string>
#include <cassert>

using namespace AOC::day09;

int main(int argc, char const *argv[])
{
    const std::string input_path = "2023/day09/input.txt";
    std::ifstream ifs;
    ifs = std::ifstream(input_path);
    std::cout << "Part 1: " << part1(ifs) << std::endl;

    ifs = std::ifstream(input_path);
    std::cout << "Part 2: " << part2(ifs) << std::endl;
    return 0;
}
