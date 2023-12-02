#include "aoc/app.hpp"

namespace aoc {

App::App(const std::string& app_description, const std::string& app_name)
    : CLI::App(app_description, app_name) {
  add_option("-i,--input", input, "Input file")
      ->required()
      ->check(CLI::ExistingFile);

  static std::map<std::string, spdlog::level::level_enum> level_maps = {
      {"trace", spdlog::level::trace}, {"debug", spdlog::level::debug},
      {"info", spdlog::level::info},   {"warning", spdlog::level::warn},
      {"warn", spdlog::level::warn},   {"error", spdlog::level::err},
      {"err", spdlog::level::err},     {"critical", spdlog::level::critical}};
  add_option("-l,--log-level", log_level, "Log level")
      ->transform(CLI::CheckedTransformer(level_maps));
}

}  // namespace aoc
