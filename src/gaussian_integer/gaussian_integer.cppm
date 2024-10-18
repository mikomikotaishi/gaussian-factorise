export module gaussianInteger;

import <algorithm>;
import <cctype>;
import <climits>;
import <cmath>;
import <iostream>;
import <regex>;
import <stdexcept>;
import <string>;
import <vector>;

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

export GaussianInteger fromString(const std::string& input);
export long flooredSqrt(long n);
export std::ostream& operator<<(std::ostream& out, const GaussianInteger& g);
