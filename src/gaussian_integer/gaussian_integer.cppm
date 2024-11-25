module;

#if !defined(_MODULES_STD) && !defined(_MODULES_LIB)
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#endif

export module gaussianInteger;

#if defined(_MODULES_STD)
import std;
#elif defined(_MODULES_LIB)
import <algorithm>;
import <cctype>;
import <climits>;
import <cmath>;
import <limits>;
import <iostream>;
import <regex>;
import <stdexcept>;
import <string>;
import <vector>;
#endif

export const std::regex REAL_REGEX{"([+-]?\\d+)$"};
export const std::regex IMAG_REGEX{"([+-]?)(\\d*)([ij]?)$"};
export const std::regex REAL_IMAG_REGEX{"([+-]?\\d+)([+-])(\\d*)([ij]?)$"};
export const std::regex IMAG_REAL_REGEX{"([+-]?)(\\d*)([ij])([+-]\\d+)$"};

export class GaussianInteger {
    int real;
    int imag;
  public:
    // Constructors
    GaussianInteger();
    GaussianInteger(int real, int imag);
    GaussianInteger(long real, long imag);

    // Accessors
    int getReal() const;
    int getImag() const;
    void setReal(int real);
    void setImag(int imag);

    // Operators
    GaussianInteger operator+(const GaussianInteger& rhs) const;
    GaussianInteger operator-(const GaussianInteger& rhs) const;
    GaussianInteger operator-() const;
    GaussianInteger operator*(const GaussianInteger& rhs) const;
    GaussianInteger operator/(const GaussianInteger& rhs) const;
    bool operator==(const GaussianInteger& rhs) const;
    bool operator!=(const GaussianInteger& rhs) const;

    // Methods
    std::string toString() const;
    GaussianInteger conjugate() const;
    long norm() const;
    std::vector<GaussianInteger> factorise() const;
    GaussianInteger findPrimeFactor() const;
};

export std::ostream& operator<<(std::ostream& out, const GaussianInteger& g);

export namespace Manipulator {
    GaussianInteger fromString(const std::string& input);
    long flooredSqrt(long n);
}