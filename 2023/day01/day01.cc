#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <istream>
#include <vector>
#include <stack>
#include <ranges>
#include <set>
#include <functional>

namespace AOC {
namespace day01 {

int get_calibration_value(std::string line, bool include_words = false) {
    std::string_view line_view = std::string_view(line);
    int first_digit = -1;
    int last_digit = -1;

    const char* chars = line.c_str();
    for (int i = 0; i < line.size(); i++) {
        const char c = chars[i];
        if (c >= '0' && c <= '9') {
            int digit = c - '0';
            if (first_digit == -1) {
                first_digit = digit;
            }
            last_digit = digit;
        } else if (include_words) {
            static const std::vector<std::string> words {
                "one",
                "two",
                "three",
                "four",
                "five",
                "six",
                "seven",
                "eight",
                "nine",
            };

            for (int j = 0; j < words.size(); j++) {
                const int digit = j+1;
                const std::string& word = words[j];
                if (line_view.substr(i).starts_with(word)) {
                    if (first_digit == -1) {
                        first_digit = digit;
                    }
                    last_digit = digit;
                }
            }
        }
    }

    return first_digit * 10 + last_digit;
}

std::string part1(std::basic_istream<char>& in) {
    int sum = 0;
    std::string line;
    while (std::getline(in, line)) {
        sum += get_calibration_value(line);
    }

    return std::to_string(sum);
}

std::string part2(std::basic_istream<char>& in) {
    int sum = 0;
    std::string line;
    while (std::getline(in, line)) {
        sum += get_calibration_value(line, /* include_words= */ true);
    }

    return std::to_string(sum);
}

}  // namespace day01
}  // namespace AOC
