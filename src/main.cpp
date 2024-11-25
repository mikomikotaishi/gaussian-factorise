#if !defined(_MODULES_STD) && !defined(_MODULES_LIB)
#include <iostream>
#include <print>
#include <random>
#endif

#ifdef _MODULES
import gaussianInteger;
import inputParser;
#else 
#include "../include/gaussian_integer/gaussian_integer.hpp"
#include "../include/utility/inputparser.hpp"
#endif

#if defined(_MODULES_STD)
import std;
#elif defined(_MODULES_LIB)
import <iostream>;
import <print>;
import <random>;
#endif

namespace Utility {
    int getRandomInt(int min, int max) {
        // Returns a random integer in the range [min, max]
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(min, max);
        return distribution(gen);
    }

    void printHelp() {
        std::println("Gaussian Integer Factoriser (C++)");
        std::println("Usage:");
        std::println("    ./gaussian-factorise");
        std::println("    ./gaussian-factorise <-h/-help/h/help>");
        std::println("    ./gaussian-factorise <-r/-random/r/random>");
        std::println("    ./gaussian-factorise [integer]");
        std::print("\n");
        std::println("If no argument is given, the program will prompt the user for an Gaussian integer.");
        std::println("If the provided Gaussian integer is \"random\" or \"r\", the program will generate a random integer.");
        std::println("Otherwise, the argument will be interpreted as an integer.");
        std::print("\n");
        std::println("The program will then factorise the integer into Gaussian primes.");
        std::print("\n");
        std::println("Also accepts j as the imaginary unit. a + bi and bi + a are both accepted forms.");
        std::print("\n");
        std::println("Examples:");
        std::println("    ./gaussian-factorise 2 ");
        std::println("        Output: (-i)(1 + i)(1 + i)");
        std::println("    ./gaussian-factorise 7");
        std::println("        Output: (7)");
        std::println("    ./gaussian-factorise 425 - 470i");
        std::println("        Output: (-1)(1 + 2i)(2 + i)(94 + 85i)");
    }
}

int main(int argc, char** argv) {
    std::string arg;
    std::vector<std::string> validOptions = {"--random", "-random", "random", "--r", "-r", "r", "--help", "-help", "help", "--h", "-h", "h"};
    try {
        if (argc == 1) {
            while (true) {
                std::println("Enter a Gaussian integer: ");
                std::string input;
                std::getline(std::cin, input);
                GaussianInteger g;
                if (input == "quit" || input == "q") 
                    break;
                else if (input == "help" || input == "h") 
                    Utility::printHelp();
                else if (input == "random" || input == "r") {
                    int real = Utility::getRandomInt(-1000, 1000);
                    int imag = Utility::getRandomInt(-1000, 1000);
                    g = GaussianInteger(real, imag);
                } else {
                    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
                    g = Manipulator::fromString(input);
                }
                std::cout << "Gaussian integer: " << g << std::endl;
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                    std::cout << ("(" + factor.toString() + ")");
                std::print("\n");
            }
        } else {
            InputParser ip(argc, argv, validOptions);
            if (ip.optionExists("--help") || ip.optionExists("-help") || ip.optionExists("help") || ip.optionExists("--h") || ip.optionExists("-h") || ip.optionExists("h"))
                Utility::printHelp();
            else if (ip.optionExists("--random") || ip.optionExists("-random") || ip.optionExists("random") || ip.optionExists("--r") || ip.optionExists("-r") || ip.optionExists("r")) {
                int real = Utility::getRandomInt(-1000, 1000);
                int imag = Utility::getRandomInt(-1000, 1000);
                GaussianInteger g = GaussianInteger(real, imag);
                std::println("Enter a Gaussian integer: ");
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                    std::cout << ("(" + factor.toString() + ")");
                std::print("\n");
            } else {
                std::string input = "";
                for (int i = 1; i < argc; ++i) 
                    input += argv[i];
                input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
                GaussianInteger g = Manipulator::fromString(input);
                std::cout << "Gaussian integer: " << g << std::endl;
                std::vector<GaussianInteger> factors = g.factorise();
                for (const GaussianInteger& factor: factors) 
                    std::cout << ("(" + factor.toString() + ")");
                std::print("\n");
            }
        }
    } catch (std::invalid_argument& e) {
        std::println(stderr, "Invalid argument: {}", e.what());
        return 1;
    } catch (std::overflow_error& e) {
        std::println(stderr, "Overflow error: {}", e.what());
        return 2;
    } catch (std::runtime_error& e) {
        std::println(stderr, "Runtime error: {}", e.what());
        return 3;
    } catch (...) {
        std::println(stderr, "Unknown error");
        return -1;
    }
    return 0;
}
