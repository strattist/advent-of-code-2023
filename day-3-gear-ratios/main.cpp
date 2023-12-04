#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <cstdint>
#include <numeric>
#include <optional>
#include <string>

#include "aoc/app.hpp"

using Pos = std::pair<int, int>;

Pos move_position(const Pos& pos, int dy, int dx) {
  return std::make_pair(pos.first + dy, pos.second + dx);
}

std::pair<Pos, Pos> get_adjacent_positions(const Pos& pos,
                                           const std::string& value) {
  return std::make_pair(move_position(pos, -1, -1),
                        move_position(pos, 1, int(value.size())));
}

int32_t value_from_string(const std::string& str) { return std::stoi(str); }

std::map<Pos, char> get_symbols_in_roi(
    const Pos& start, const Pos& end, const std::map<Pos, char>& symbols,
    std::optional<char> selected_symbol = {}) {
  std::map<Pos, char> symbols_in_roi;
  for (int y = start.first; y <= end.first; ++y) {
    for (int x = start.second; x <= end.second; ++x) {
      Pos p = std::make_pair(y, x);
      try {
        if (selected_symbol) {
          if (symbols.at(p) == selected_symbol.value()) {
            symbols_in_roi[p] = selected_symbol.value();
          }
        } else {
          symbols_in_roi[p] = symbols.at(p);
        }
      } catch (const std::exception& e) {
        // no symbol in p
      }
    }
  }
  return symbols_in_roi;
}

bool has_symbol_in_roi(const Pos& start, const Pos& end,
                       const std::map<Pos, char>& symbols) {
  std::map<Pos, char> symbols_in_roi = get_symbols_in_roi(start, end, symbols);
  return symbols_in_roi.size() > 0;
}

struct Schematic {
  std::vector<int> get_numbers_with_symbol() const {
    std::vector<int> parts;
    for (const auto& [pos, value] : values) {
      auto [start, end] = get_adjacent_positions(pos, value);
      if (has_symbol_in_roi(start, end, grid)) {
        parts.push_back(value_from_string(value));
      }
    }
    return parts;
  }

  std::vector<int> get_gear_ratios() const {
    std::vector<int> gear_ratios;
    std::map<Pos, std::vector<int>> number_per_gears;
    for (const auto& [pos, value] : values) {
      auto [start, end] = get_adjacent_positions(pos, value);
      auto gears_around_value = get_symbols_in_roi(start, end, grid, '*');
      for (const auto& [gear_pos, _] : gears_around_value) {
        number_per_gears[gear_pos].push_back(value_from_string(value));
      }
    }

    for (const auto& [_, gears] : number_per_gears) {
      if (gears.size() >= 2) {
        gear_ratios.push_back(std::accumulate(
            std::begin(gears), std::end(gears), 1, std::multiplies<int>()));
      }
    }
    return gear_ratios;
  }

  std::map<Pos, std::string> values;
  std::map<Pos, char> grid;
};

void update_values_from_line_and_replace_by_point(int y, std::string& line,
                                                  Schematic& schematic) {
  auto get_next_value = [](std::string& line) {
    auto idx = line.find_first_of("0123456789");
    auto start = idx;
    std::string value = "";
    if (idx != std::string::npos) {
      while (isdigit(line[idx])) {
        value.push_back(line[idx]);
        line[idx++] = '.';
      }
    }
    return std::make_pair(start, value);
  };

  bool keep_parsing = true;
  while (keep_parsing) {
    auto [x, value] = get_next_value(line);
    if (value != "") {
      spdlog::trace("Got new value '{}' in ({}, {})", value, y, x);
      schematic.values[std::make_pair(y, x)] = value;
    } else {
      keep_parsing = false;
    }
  }
}

void update_grid_from_line(int y, std::string& line, Schematic& schematic) {
  bool keep_parsing = true;
  while (keep_parsing) {
    auto idx = line.find_first_not_of(".");
    if (idx != std::string::npos) {
      schematic.grid[std::make_pair(y, idx)] = line[idx];
      line[idx] = '.';
    } else {
      keep_parsing = false;
    }
  }
}

Schematic parse_input(const std::filesystem::path& input) {
  std::ifstream f(input);
  Schematic schematic;
  if (f.is_open()) {
    std::string line;
    spdlog::debug("Successfully opened {}", input.string());
    int y = 0;
    while (std::getline(f, line)) {
      update_values_from_line_and_replace_by_point(y, line, schematic);
      update_grid_from_line(y, line, schematic);
      y++;
    }
  } else {
    spdlog::error("Failed to open {}", input.string());
  }
  return schematic;
}

int main(int argc, char* argv[]) {
  aoc::App app("Advent of code 2023 Day 3: Gear Ratios");
  CLI11_PARSE(app, argc, argv);
  spdlog::set_level(app.log_level);

  Schematic schematic = parse_input(app.input);

  std::vector<int> parts = schematic.get_numbers_with_symbol();
  int sum_of_parts = std::accumulate(std::begin(parts), std::end(parts), 0);
  spdlog::info("Sum of parts = {}", sum_of_parts);

  std::vector<int> gear_ratios = schematic.get_gear_ratios();
  int sum_of_gear_ratios =
      std::accumulate(std::begin(gear_ratios), std::end(gear_ratios), 0);
  spdlog::info("Sum of gear ratios = {}", sum_of_gear_ratios);

  return 0;
}
