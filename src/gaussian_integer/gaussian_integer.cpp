#ifdef _MODULES
module gaussianInteger;
#else
#include "../../include/gaussian_integer/gaussian_integer.hpp"
#endif

GaussianInteger::GaussianInteger(): 
    real{0}, imag{0} {}

GaussianInteger::GaussianInteger(int real, int imag): 
    real{real}, imag{imag} {
    if (real == std::numeric_limits<int>::min() || real == std::numeric_limits<int>::max() || imag == std::numeric_limits<int>::min() || imag == std::numeric_limits<int>::max())
        throw std::overflow_error("GaussianInteger overflow");
}

GaussianInteger::GaussianInteger(long real, long imag) {
    if (real <= std::numeric_limits<int>::min() || real >= std::numeric_limits<int>::max() || imag <= std::numeric_limits<int>::min() || imag >= std::numeric_limits<int>::max())
        throw std::overflow_error("GaussianInteger overflow");
    else {
        this->real = static_cast<int>(real);
        this->imag = static_cast<int>(imag);
    }
}

// Accessors
int GaussianInteger::getReal() const { return real; }

int GaussianInteger::getImag() const { return imag; }

void GaussianInteger::setReal(int real) { 
    if (real == std::numeric_limits<int>::min() || real == std::numeric_limits<int>::max())
        throw std::overflow_error("GaussianInteger overflow");
    else
        this->real = real; 
}

void GaussianInteger::setImag(int imag) { 
    if (imag == std::numeric_limits<int>::min() || imag == std::numeric_limits<int>::max())
        throw std::overflow_error("GaussianInteger overflow");
    else
        this->imag = imag; 
}

// Operators
GaussianInteger GaussianInteger::operator+(const GaussianInteger& rhs) const {
    long realResult = static_cast<long>(real) + rhs.real;
    long imagResult = static_cast<long>(imag) + rhs.imag;
    try {
        return GaussianInteger(realResult, imagResult);
    } catch (std::overflow_error& e) {
        throw std::overflow_error("GaussianInteger overflow");
    }
}

GaussianInteger GaussianInteger::operator-(const GaussianInteger& rhs) const { return GaussianInteger(real - rhs.real, imag - rhs.imag); }

GaussianInteger GaussianInteger::operator-() const { return GaussianInteger(-real, -imag); }

GaussianInteger GaussianInteger::operator*(const GaussianInteger& rhs) const {
    long realResult = static_cast<long>(real) * rhs.real - static_cast<long>(imag) * rhs.imag;
    long imagResult = static_cast<long>(real) * rhs.imag + static_cast<long>(imag) * rhs.real;
    try {
        return GaussianInteger(realResult, imagResult);
    } catch (std::overflow_error& e) {
        throw std::overflow_error("GaussianInteger overflow");
    }
}

bool isDivisibleBy(const GaussianInteger& g1, const GaussianInteger& g2) {
    // Helper function to check if a GaussianInteger is divisible by another
    long divisorNorm = g2.norm();
    long realPart = static_cast<long>(g1.getReal());
    long imagPart = static_cast<long>(g1.getImag());
    bool divisible1 = (realPart * g2.getReal() + imagPart * g2.getImag()) % divisorNorm == 0;
    bool divisible2 = (-realPart * g2.getImag() + imagPart * g2.getReal()) % divisorNorm == 0;
    return divisible1 && divisible2;
}

GaussianInteger GaussianInteger::operator/(const GaussianInteger& rhs) const {
    if (rhs.real == 0 && rhs.imag == 0) 
        throw std::invalid_argument("Division by zero");
    else if (!isDivisibleBy(*this, rhs)) 
        throw std::invalid_argument("Division not exact");
    else {
        long realResult = (static_cast<long>(real) * rhs.real + static_cast<long>(imag) * rhs.imag) / rhs.norm();
        long imagResult = (static_cast<long>(-real) * rhs.imag + static_cast<long>(imag) * rhs.real) / rhs.norm();
        try {
            return GaussianInteger(realResult, imagResult);
        } catch (std::overflow_error& e) {
            throw std::overflow_error("GaussianInteger overflow");
        }
    }
}

bool GaussianInteger::operator==(const GaussianInteger& rhs) const { return real == rhs.real && imag == rhs.imag; }

bool GaussianInteger::operator!=(const GaussianInteger& rhs) const { return !(*this == rhs); }

std::ostream& operator<<(std::ostream& os, const GaussianInteger& g) {
    os << g.toString() << std::endl;
    return os;
}

// Methods
std::string GaussianInteger::toString() const {
    std::string realStr = std::to_string(real);
    std::string imagStr = std::to_string(imag);
    if (imag == 0) 
        return realStr;
    else if (real == 0) {
        switch (imag) {
            case 1:
                return "i";
            case -1:
                return "-i";
            default:
                return imagStr + "i";
        }
    } else if (imag == 1) 
        return realStr + " + i";
    else if (imag == -1) 
        return realStr + " - i";
    else if (imag > 0) 
        return realStr + " + " + imagStr + "i";
    else 
        return realStr + " - " + std::to_string(-imag) + "i";
}

GaussianInteger GaussianInteger::conjugate() const { return GaussianInteger(real, -imag); }

long GaussianInteger::norm() const {
    // Prevent overflow by casting to long before multiplying
    return static_cast<long>(real) * real + static_cast<long>(imag) * imag;
}

GaussianInteger GaussianInteger::findPrimeFactor() const {
    // Find a Gaussian prime factor of a GaussianInteger
    long n = norm();
    if (n % 2 == 0) 
        return GaussianInteger(1, 1);
    else {
        long m = Manipulator::flooredSqrt(n);
        for (long i = 3; i <= m; i += 2) {
            if (n % i == 0) {
                if (i % 4 == 3) 
                    return GaussianInteger(i, 0l);
                else {
                    for (long realResult = Manipulator::flooredSqrt(i); realResult > 0; --realResult) {
                        long imagResult = Manipulator::flooredSqrt(i - realResult * realResult);
                        if (realResult * realResult + imagResult * imagResult == i) {
                            if (realResult > std::numeric_limits<int>::max() || imagResult > std::numeric_limits<int>::max()) 
                                throw std::overflow_error("GaussianInteger overflow");
                            else if (isDivisibleBy(*this, GaussianInteger(static_cast<int>(realResult), static_cast<int>(imagResult)))) 
                                return GaussianInteger(static_cast<int>(realResult), static_cast<int>(imagResult));
                        }
                    }
                }
            }
        }
        GaussianInteger g = *this;
        while (g.real < 0 || g.imag < 0) 
            g = g * GaussianInteger(0, 1);
        return g;
    }
}

std::vector<GaussianInteger> GaussianInteger::factorise() const {
    // Factorise a GaussianInteger into Gaussian primes
    std::vector<GaussianInteger> factors;
    GaussianInteger g = *this;
    if (g.norm() <= 1) {
        factors.push_back(g);
        return factors;
    }
    GaussianInteger gCheck = GaussianInteger(1, 0);
    while (g.norm() > 1) {
        GaussianInteger prime = g.findPrimeFactor();
        factors.push_back(prime);
        g = g / prime;
        gCheck = gCheck * prime;
    }
    gCheck = gCheck * g;
    if (g.norm() != 1 || gCheck != *this) 
        throw std::runtime_error("Factorisation failed");
    if (g.getReal() != 1)
        factors.push_back(g);
    std::sort(factors.begin(), factors.end(), [](const GaussianInteger& g1, const GaussianInteger& g2) -> bool {
        return (g1.norm() < g2.norm()) || ((g1.norm() == g2.norm()) && (g1.getReal() < g2.getReal()));
    });
    return factors;
}

namespace Manipulator {
    long flooredSqrt(long n) {
        // Find the square root of a long
        if (n < 0)
            throw std::invalid_argument("Cannot find square root of negative number");
        else
            return static_cast<long>(std::sqrt(n));
    }

    GaussianInteger fromString(const std::string& input) {
        // Parse a string into a GaussianInteger
        long realComp;
        long imagComp;
        std::smatch match;
        if (std::regex_match(input, match, REAL_REGEX)) {
            realComp = std::stoi(match[1]);
            imagComp = 0;
        } else if (std::regex_match(input, match, IMAG_REGEX)) {
            realComp = 0;
            imagComp = (match[1] == "-" ? -1 : 1) * (match[2].str().empty() ? 1 : std::stoi(match[2]));
        } else if (std::regex_match(input, match, REAL_IMAG_REGEX)) {
            realComp = std::stoi(match[1]);
            imagComp = (match[2] == "-" ? -1 : 1) * (match[3].str().empty() ? 1 : std::stoi(match[3]));
        } else if (std::regex_match(input, match, IMAG_REAL_REGEX)) {
            realComp = std::stoi(match[4]);
            imagComp = (match[1] == "-" ? -1 : 1) * (match[2].str().empty() ? 1 : std::stoi(match[2]));
        } else throw std::invalid_argument("Invalid input: " + input);
        if (realComp <= std::numeric_limits<int>::min() || realComp >= std::numeric_limits<int>::max() || imagComp <= std::numeric_limits<int>::min() || imagComp >= std::numeric_limits<int>::max()) 
            throw std::overflow_error("GaussianInteger overflow");
        else 
            return GaussianInteger(realComp, imagComp);
    }
}