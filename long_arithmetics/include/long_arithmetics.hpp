#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace LongArithmetics
{
    struct BigNum
    {
    public:
        BigNum();
        explicit BigNum(const std::string &number);
        // explicit BigNum(const std::string &number, size_t precision = static_cast<size_t>(-1));
        // explicit BigNum(const char *number, size_t precision = static_cast<size_t>(-1));
        // explicit BigNum(long long number, size_t precision = static_cast<size_t>(-1));

        bool operator==(const BigNum &other) const;
        bool operator!=(const BigNum &other) const;
        bool operator<(const BigNum &other) const;
        bool operator>(const BigNum &other) const;
        bool operator<=(const BigNum &other) const;
        bool operator>=(const BigNum &other) const;

        BigNum &operator+=(const BigNum &a);
        BigNum &operator-=(const BigNum &a);
        BigNum &operator*=(const BigNum &a);
        BigNum &operator/=(const BigNum &a);

        friend BigNum operator+(const BigNum &a, const BigNum &b);
        friend BigNum operator-(const BigNum &a, const BigNum &b);
        friend BigNum operator*(const BigNum &a, const BigNum &b);
        friend BigNum operator/(const BigNum &a, const BigNum &b);

        BigNum operator-() const;

        bool is_zero() const;
        int get_digit(int i) const;

        friend std::ostream &operator<<(std::ostream &out, const BigNum &a);

    private:
        std::vector<int> digits;
        size_t precision;
        int sign; // 1 for >=0, -1 for <0

        void remove_zeros();
    };

    // BigNum operator"" _bn(const char *str, std::size_t);
    // BigNum operator"" _bn(long double num);
} // namespace LongArithmetics