#include "day02.cc"

#include <string>
#include <cassert>

using namespace AOC::day02;

int main(int argc, char const *argv[])
{
    const std::string input_path = "2023/day02/input.txt";
    std::ifstream ifs;
    ifs = std::ifstream(input_path);
    std::cout << part1(ifs) << std::endl;

    ifs = std::ifstream(input_path);
    std::cout << part2(ifs) << std::endl;
    return 0;
}
