import <array>;
import <iostream>;
import <random>;
import <stdexcept>;
import <string>;
import <vector>;

import gaussianInteger;
import inputParser;

int getRandomInt(int min, int max) {
    // Returns a random integer in the range [min, max]
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}

void printHelp() {
    std::cout << "Gaussian Integer Factoriser (C++)\n";
    std::cout << "Usage:\n";
    std::cout << "    ./gaussian-factorise\n";
    std::cout << "    ./gaussian-factorise <-h/-help/h/help>\n";
    std::cout << "    ./gaussian-factorise <-r/-random/r/random>\n";
    std::cout << "    ./gaussian-factorise [integer]\n";
    std::cout << "\n";
    std::cout << "If no argument is given, the program will prompt the user for an Gaussian integer.\n";
    std::cout << "If the provided Gaussian integer is \"random\" or \"r\", the program will generate a random integer.\n";
    std::cout << "Otherwise, the argument will be interpreted as an integer.\n";
    std::cout << "\n";
    std::cout << "The program will then factorise the integer into Gaussian primes.\n";
    std::cout << "\n";
    std::cout << "Also accepts j as the imaginary unit. a + bi and bi + a are both accepted forms.\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "    ./gaussian-factorise 2 \n";
    std::cout << "        Output: (-i)(1 + i)(1 + i)\n";
    std::cout << "    ./gaussian-factorise 7\n";
    std::cout << "        Output: (7)\n";
    std::cout << "    ./gaussian-factorise 425 - 470i\n";
    std::cout << "        Output: (-1)(1 + 2i)(2 + i)(94 + 85i)\n";
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    std::string arg;
    std::array<std::string, 12> validOptions = {"--random", "-random", "random", "--r", "-r", "r", "--help", "-help", "help", "--h", "-h", "h"};
    try {
        if (argc == 1) {
            while (true) {
                std::cout << "Enter a Gaussian integer: " << std::endl;
                std::string input;
                std::getline(std::cin, input);
                GaussianInteger g;
                if (input == "quit" || input == "q") 
                    break;
                else if (input == "help" || input == "h") 
                    printHelp();
                else if (input == "random" || input == "r") {
                    int real = getRandomInt(-1000, 1000);
                    int imag = getRandomInt(-1000, 1000);
                    g = GaussianInteger(real, imag);
                } else {
                    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
                    g = fromString(input);
                }
                std::cout << "Gaussian integer: " << g << std::endl;
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                std::cout << ("(" + factor.toString() + ")");
                std::cout << std::endl;
            }
        } else {
            InputParser ip(argc, argv, validOptions);
            if (ip.optionExists("--help") || ip.optionExists("-help") || ip.optionExists("help") || ip.optionExists("--h") || ip.optionExists("-h") || ip.optionExists("h"))
                printHelp();
            else if (ip.optionExists("--random") || ip.optionExists("-random") || ip.optionExists("random") || ip.optionExists("--r") || ip.optionExists("-r") || ip.optionExists("r")) {
                int real = getRandomInt(-1000, 1000);
                int imag = getRandomInt(-1000, 1000);
                GaussianInteger g = GaussianInteger(real, imag);
                std::cout << "Gaussian integer: " << g << std::endl;
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                    std::cout << ("(" + factor.toString() + ")");
                std::cout << std::endl;
            } else {
                std::string input = "";
                for (int i = 1; i < argc; ++i) 
                    input += argv[i];
                input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
                GaussianInteger g = fromString(input);
                std::cout << "Gaussian integer: " << g << std::endl;
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                    std::cout << ("(" + factor.toString() + ")");
                std::cout << std::endl;
            }
        }
    } catch (std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    } catch (std::overflow_error& e) {
        std::cerr << "Overflow error: " << e.what() << std::endl;
        return 2;
    } catch (std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 3;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return -1;
    }
    return 0;
}
