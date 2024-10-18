module inputParser;

bool isInteger(const std::string& s) {
    try {
        std::stoi(s);
        return true;
    } catch (...) {
        return false;
    }
}
 
// Constructor
InputParser::InputParser(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) 
        args.push_back(std::string(argv[i]));
}

InputParser::InputParser(int argc, char* argv[], const std::vector<std::string>& validOptions): 
    validOptions(validOptions) {
    #ifdef DEBUG
    assert(argc >= 1);
    #endif
    bool foundValue = false;
    for (int i = 1; i < argc; ++i) 
        args.push_back(std::string(argv[i]));
    for (const std::string& arg: args) {
        if (std::find(validOptions.begin(), validOptions.end(), arg) == validOptions.end()) {
            if (isInteger(arg) && !foundValue) {
                foundValue = true;
                continue;
            }
            throw std::invalid_argument("Invalid option: " + arg);
        }
    }
}

// Methods
std::string InputParser::getOptionValue(const std::string& option) const {
    std::vector<std::string>::const_iterator it = std::find(args.begin(), args.end(), option);
    if (it != args.end() && ++it != args.end()) 
        return *it;
    return "";
}

bool InputParser::optionExists(const std::string& option) const {
    return std::find(args.begin(), args.end(), option) != args.end();
}