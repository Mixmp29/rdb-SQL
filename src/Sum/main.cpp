#include <cxxopts.hpp>
#include <sum.hpp>

#include <iostream>

int main(int argc, char** argv) {
  cxxopts::Options options("sum");

  try {
    // clang-format off
    options.add_options()
      ("first", "Double number", cxxopts::value<double>())
      ("second", "Double number", cxxopts::value<double>());
    // clang-format on

    const auto result = options.parse(argc, argv);

    if (result.count("first") != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    if (result.count("second") != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    const auto first = result["first"].as<double>();
    const auto second = result["second"].as<double>();

    std::cout << sum(first, second) << "\n";
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}