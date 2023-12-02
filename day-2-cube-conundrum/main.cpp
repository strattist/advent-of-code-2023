#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <stdexcept>

#include "aoc/app.hpp"

struct Draw {
  int red{0};
  int green{0};
  int blue{0};

  void update(int n, std::string name) {
    if (name == "red") {
      if (red != 0) std::runtime_error("Updating red that is not 0");
      red += n;
    } else if (name == "green") {
      if (green != 0) std::runtime_error("Updating green that is not 0");
      green += n;
    } else if (name == "blue") {
      if (blue != 0) std::runtime_error("Updating blue that is not 0");
      blue += n;
    } else {
      throw std::invalid_argument("Bad color name");
    }
    spdlog::trace("        R{}, G{}, B{}", red, green, blue);
  }

  int product() const { return red * green * blue; }
};

struct Game {
  int id;
  std::vector<Draw> rgb;

  int power() const {
    Draw minimum = rgb[0];
    for (int i = 1; i < rgb.size(); ++i) {
      if (rgb[i].red > minimum.red) minimum.red = rgb[i].red;
      if (rgb[i].green > minimum.green) minimum.green = rgb[i].green;
      if (rgb[i].blue > minimum.blue) minimum.blue = rgb[i].blue;
    }
    return minimum.product();
  }

  bool can_draw(const Draw& m) const {
    for (const auto& draw : rgb) {
      if (draw.red > m.red || draw.green > m.green || draw.blue > m.blue)
        return false;
    }
    return true;
  }
};

Game parse_line(const std::string& line) {
  std::string line_copy = line;

  Game game;
  std::string placeholder;
  std::stringstream ss_id(line);
  ss_id >> placeholder >> game.id;

  auto start = line_copy.find_first_of(":");
  std::stringstream ss(line_copy.substr(start + 1));
  std::string current_rgb;
  while (std::getline(ss, current_rgb, ';')) {
    std::stringstream ss_rgb(current_rgb);
    std::string current_color;
    Draw cubes;
    while (std::getline(ss_rgb, current_color, ',')) {
      int n;
      std::string name;
      std::stringstream ss_color(current_color);
      ss_color >> n >> name;
      cubes.update(n, name);
    }
    game.rgb.push_back(cubes);
  }
  return game;
}

std::vector<Game> parse_input(const std::filesystem::path& input) {
  std::ifstream f(input);
  std::vector<Game> games;
  if (f.is_open()) {
    std::string line;
    spdlog::debug("Successfully opened {}", input.string());
    int sum_of_calibration_values = 0;
    while (std::getline(f, line)) {
      Game game = parse_line(line);
      if (game.rgb.size() > 0) games.push_back(game);
    }

  } else {
    spdlog::error("Failed to open {}", input.string());
  }
  return games;
}

int main(int argc, char* argv[]) {
  aoc::App app("Advent of code 2023 Day 2: Cube Conundrum");
  CLI11_PARSE(app, argc, argv);
  spdlog::set_level(app.log_level);

  std::vector<Game> games = parse_input(app.input);
  int sum = 0;
  int power = 0;
  Draw maximum{.red = 12, .green = 13, .blue = 14};
  for (const auto& game : games) {
    power += game.power();
    if (game.can_draw(maximum)) {
      sum += game.id;
    }
  }
  spdlog::info("Sum of possible games = {}", sum);
  spdlog::info("Power of all games = {}", power);
  return 0;
}
