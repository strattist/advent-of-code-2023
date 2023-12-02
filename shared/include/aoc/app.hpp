#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <filesystem>
#include <string>

namespace aoc {

class App : public CLI::App {
 public:
  App(const std::string &app_description = "",
      const std::string &app_name = "");

  std::filesystem::path input;
  spdlog::level::level_enum log_level{spdlog::level::info};
};

}  // namespace aoc
