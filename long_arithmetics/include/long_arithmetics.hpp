#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace LongArithmetics
{
    constexpr size_t SIMPLE_LIMIT = 128;
    constexpr size_t divDigits = 100;
    struct BigNum
    {
    public:
        BigNum();
        explicit BigNum(long double d);
        explicit BigNum(const std::string &number);
        BigNum(const BigNum &other);
        BigNum(BigNum &&other);

        bool operator==(const BigNum &other) const;
        bool operator!=(const BigNum &other) const;
        bool operator<(const BigNum &other) const;
        bool operator>(const BigNum &other) const;
        bool operator<=(const BigNum &other) const;
        bool operator>=(const BigNum &other) const;

        BigNum &operator=(const BigNum &other);
        BigNum &operator=(BigNum &&other);
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
        BigNum inverse() const;

        friend std::ostream &operator<<(std::ostream &out, const BigNum &a);

    private:
        std::vector<int> digits;
        int32_t precision;
        bool sign; // 0 for >=0, 1 for <0

        void remove_zeros();
    };

    std::vector<int> simple_mul(const std::vector<int> &lhs, const std::vector<int> &rhs);
    std::vector<int> karatsuba_mul(const std::vector<int> &lhs, const std::vector<int> &rhs);
    std::string to_string(const BigNum &);

    BigNum operator"" _bn(const long double num);
} // namespace LongArithmetics
