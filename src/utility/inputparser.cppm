export module inputParser;

import <algorithm>;
import <array>;
import <cassert>;
import <string>;
import <stdexcept>;
import <vector>;

export class InputParser {
  private:
    std::vector<std::string> args;
    std::vector<std::string> validOptions; // Optional
  public:
    // Constructor
    InputParser(int argc, char* argv[]);
    InputParser(int argc, char* argv[], const std::vector<std::string>& validOptions);
    InputParser(int argc, char* argv[])

    // Methods
    std::string getOptionValue(const std::string& option) const;
    bool optionExists(const std::string& option) const;
};