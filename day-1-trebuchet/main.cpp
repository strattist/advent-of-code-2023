#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <fstream>
#include <map>
#include <string>

#include "aoc/app.hpp"

int get_calibration_value_from_first_and_last(const char& first,
                                              const char& last) {
  std::stringstream ss;
  ss << first << last;
  int value;
  ss >> value;
  return value;
}

int get_calibration_value(const std::string& line,
                          const std::map<std::string, char>& valid) {
  std::pair<int, char> first{line.size(), '0'};
  std::pair<int, char> last{-1, '0'};
  for (const auto& [to_find, val] : valid) {
    auto maybe_first = line.find(to_find);
    if (maybe_first < first.first && maybe_first != std::string::npos)
      first = std::make_pair(int(maybe_first), val);

    auto maybe_last = line.rfind(to_find);
    if (int(maybe_last) > int(last.first) && maybe_last != std::string::npos) {
      last = std::make_pair(int(maybe_last), val);
    }
  }
  return get_calibration_value_from_first_and_last(first.second, last.second);
}

int get_calibration_values(const std::filesystem::path& input,
                           const std::map<std::string, char>& valid) {
  std::ifstream f(input);
  if (f.is_open()) {
    std::string line;
    spdlog::debug("Successfully opened {}", input.string());
    int sum_of_calibration_values = 0;
    while (std::getline(f, line)) {
      auto calibration_value = get_calibration_value(line, valid);
      spdlog::debug("{} -> {}", line, calibration_value);
      sum_of_calibration_values += calibration_value;
    }
    return sum_of_calibration_values;
  } else {
    spdlog::error("Failed to open {}", input.string());
    return -1;
  }
}

int main(int argc, char* argv[]) {
  aoc::App app("Advent of code 2023 Day 1: Trebuchet?!");
  CLI11_PARSE(app, argc, argv);
  spdlog::set_level(app.log_level);

  std::map<std::string, char> digits{
      {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'},
      {"5", '5'}, {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'}};
  std::map<std::string, char> digits_and_names{
      {"one", '1'},   {"two", '2'},   {"three", '3'},
      {"four", '4'},  {"five", '5'},  {"six", '6'},
      {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};
  digits_and_names.insert(std::begin(digits), std::end(digits));

  auto value_from_digits = get_calibration_values(app.input, digits);
  auto value_from_digits_and_names =
      get_calibration_values(app.input, digits_and_names);

  spdlog::info("Sum of calibration values (digits) = {}", value_from_digits);
  spdlog::info("Sum of calibration values (digits and names) = {}",
               value_from_digits_and_names);
  return 0;
}
