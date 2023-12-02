#include "day01.cc"

#include <string>
#include <cassert>

using namespace AOC::day01;

int main(int argc, char const *argv[])
{
    const std::string input_path = "2023/day01/input.txt";
    std::ifstream ifs;
    ifs = std::ifstream(input_path);
    std::cout << part1(ifs) << std::endl;

    ifs = std::ifstream(input_path);
    std::cout << part2(ifs) << std::endl;
    return 0;
}
