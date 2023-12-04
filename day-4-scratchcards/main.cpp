#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>

#include "aoc/app.hpp"
#include "aoc/utils.hpp"

std::tuple<std::vector<int>, std::vector<int>> parse_numbers(
    const std::string& numbers) {
  std::vector<int> winning_numbers;
  std::vector<int> hand;

  auto v = aoc::split_string_by_character(numbers, '|');
  auto swinning = aoc::split_string_by_character(v[0], ' ');
  auto shand = aoc::split_string_by_character(v[1], ' ');
  std::transform(std::begin(swinning), std::end(swinning),
                 std::back_inserter(winning_numbers),
                 [](const std::string& s) { return std::stoi(s); });
  std::transform(std::begin(shand), std::end(shand), std::back_inserter(hand),
                 [](const std::string& s) { return std::stoi(s); });
  return std::make_tuple(winning_numbers, hand);
}

std::tuple<int, std::vector<int>, std::vector<int>> parse_line_for_card(
    const std::string& line) {
  int card_id{-1};

  auto v = aoc::split_string_by_character(line, ':');

  std::stringstream sids(v[0]);
  std::string placeholder;
  sids >> placeholder >> card_id;

  auto [winning_numbers, hand] = parse_numbers(v[1]);
  return std::make_tuple(card_id, winning_numbers, hand);
}

struct Card {
  Card(const std::string& line) {
    auto [id_, winning_numbers_, hand_] = parse_line_for_card(line);
    id = id_;
    winning_numbers = winning_numbers_;
    hand = hand_;

    std::copy_if(std::begin(hand), std::end(hand),
                 std::back_inserter(winning_hand), [&](const int& v) {
                   return std::find(std::begin(winning_numbers),
                                    std::end(winning_numbers),
                                    v) != std::end(winning_numbers);
                 });
    spdlog::trace("Card {}", id);
    spdlog::trace("  winning_numbers: {}", fmt::join(winning_numbers, ", "));
    spdlog::trace("  hand           : {}", fmt::join(hand, ", "));
    spdlog::trace("  winning_hand   : {}", fmt::join(winning_hand, ", "));
  }

  int n_points() const {
    return winning_hand.empty() ? 0 : std::pow(2, winning_hand.size() - 1);
  }

  int id;
  std::vector<int> winning_numbers;
  std::vector<int> hand;
  std::vector<int> winning_hand;
};

std::vector<Card> parse_cards(const std::filesystem::path& input) {
  std::ifstream f(input);
  std::vector<Card> cards;
  if (f.is_open()) {
    spdlog::debug("Successfully opened {}", input.string());
    std::string line;
    while (std::getline(f, line)) {
      if (!line.empty()) cards.push_back(Card(line));
    }
  } else {
    spdlog::error("Failed to open {}", input.string());
  }
  return cards;
}

int main(int argc, char* argv[]) {
  aoc::App app("Advent of code 2023 Day 3: Gear Ratios");
  CLI11_PARSE(app, argc, argv);
  spdlog::set_level(app.log_level);

  auto cards = parse_cards(app.input);

  auto n_points = std::accumulate(
      std::begin(cards), std::end(cards), 0,
      [&](int sum, const Card& c) { return sum + c.n_points(); });
  spdlog::info("N Points in winning hands = {}", n_points);

  std::vector<int> cards_copies(cards.size(), 1);
  for (int i = 0; i < cards.size(); ++i) {
    for (int j = i + 1; j < i + 1 + cards[i].winning_hand.size(); ++j)
      cards_copies[j] += cards_copies[i];
  }
  auto n_copies =
      std::accumulate(std::begin(cards_copies), std::end(cards_copies), 0);
  spdlog::info("N Cards copies = {}", n_copies);

  return 0;
}
