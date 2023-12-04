#include "aoc/utils.hpp"

#include <sstream>

namespace aoc {

std::vector<std::string> split_string_by_character(const std::string& s,
                                                   char splitter) {
  std::stringstream ss(s);

  std::vector<std::string> elmts;
  std::string elmt;
  while (std::getline(ss, elmt, splitter)) {
    if (!elmt.empty()) elmts.push_back(elmt);
  }
  return elmts;
}

}  // namespace aoc
