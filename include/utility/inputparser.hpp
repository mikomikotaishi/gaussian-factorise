#pragma once
#include <algorithm>
#include <cassert>
#include <string>
#include <stdexcept>
#include <vector>

class InputParser {
  private:
    std::vector<std::string> args;
    std::vector<std::string> validOptions; // Optional
  public:
    // Constructor
    InputParser(int argc, char* argv[]);
    InputParser(int argc, char* argv[], const std::vector<std::string>& validOptions);

    // Methods
    std::string getOptionValue(const std::string& option) const;
    bool optionExists(const std::string& option) const;
};
