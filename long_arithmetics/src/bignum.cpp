#include <long_arithmetics.hpp>
#include <iomanip>

constexpr int base = 1000'000'000;
constexpr int base_digits = 9;

namespace LongArithmetics
{
    BigNum::BigNum()
    {
        sign = 1;
        digits = {0};
        precision = 0;
    }

    BigNum::BigNum(const std::string &number)
    {
        size_t i = 0;
        sign = number[0] == '-' ? ++i : 0;
        precision = number.size() - i;
        digits.reserve(precision);
        while (i < number.size())
        {
            if (number[i] == '.')
            {
                precision = sign ? i - 1 : i;
            }
            else if (isdigit(number[i]))
            {
                digits.push_back(number[i] - '0');
            }
            ++i;
        }
        remove_zeros();
    }

    BigNum BigNum::operator-() const
    {
        BigNum result = *this;
        result.sign = -result.sign;
        return result;
    }

    BigNum &BigNum::operator+=(const BigNum &a)
    {
        // Ensure the vectors are of the same size
        while (digits.size() < a.digits.size())
            digits.insert(digits.begin(), 0);

        // Perform addition
        int carry = 0;
        for (int i = digits.size() - 1; i >= 0; --i)
        {
            digits[i] += (i < a.digits.size() ? a.digits[i] : 0) + carry;
            carry = (digits[i] >= 10) ? 1 : 0;
            if (carry)
                digits[i] -= 10;
        }

        // If there's still a carry after the last digit, add a new digit
        if (carry)
            digits.insert(digits.begin(), 1);

        // Adjust precision
        if (precision < a.precision)
            precision = a.precision;

        // Remove leading zeros
        while (digits.size() > 1 && digits[0] == 0)
        {
            digits.erase(digits.begin());
            --precision;
        }

        return *this;
    }

    BigNum operator+(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result += b;
        return result;
    }

    BigNum &BigNum::operator-=(const BigNum &a)
    {
        // Ensure the vectors are of the same size
        while (digits.size() < a.digits.size())
            digits.push_back(0);

        // Perform subtraction
        for (size_t i = 0; i < a.digits.size(); ++i)
        {
            if (digits[i] < a.digits[i])
            {
                digits[i + 1]--;
                digits[i] += 10;
            }
            digits[i] -= a.digits[i];
        }

        // Handle borrow for the last digit
        for (size_t i = a.digits.size(); i < digits.size(); ++i)
        {
            if (digits[i] < 0)
            {
                digits[i + 1]--;
                digits[i] += 10;
            }
            else
                break;
        }

        // Remove leading zeros
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();

        // Update precision
        precision = digits.size();

        return *this;
    }

    BigNum operator-(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result -= b;
        return result;
    }

    BigNum &BigNum::operator*=(const BigNum &a)
    {
        std::vector<int> result(this->digits.size() + a.digits.size(), 0);
        for (size_t i = 0; i < this->digits.size(); ++i)
        {
            for (size_t j = 0; j < a.digits.size(); ++j)
            {
                result[i + j] += this->digits[i] * a.digits[j];
                if (result[i + j] >= base)
                {
                    result[i + j + 1] += result[i + j] / base;
                    result[i + j] %= base;
                }
            }
        }
        this->digits = result;
        this->precision = this->precision + a.precision;
        return *this;
    }

    BigNum operator*(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result *= b;
        return result;
    }

    BigNum &BigNum::operator/=(const BigNum &a)
    {
        if (a.is_zero())
        {
            throw std::invalid_argument("Division by zero");
        }

        int precision = std::max(this->precision, a.precision);

        this->digits.resize(this->digits.size() + precision, 0);

        BigNum dividend = *this;
        BigNum divisor = a;
        dividend.sign = divisor.sign = 1;

        BigNum quotient;
        quotient.digits.resize(digits.size());

        for (int i = digits.size() - 1; i >= 0; --i)
        {
            quotient.digits[i] = dividend.digits[i] / divisor.digits[i];
            dividend.digits[i] %= divisor.digits[i];

            if (i > 0)
            {
                dividend.digits[i - 1] += dividend.digits[i] * base;
            }
        }

        *this = quotient;
        this->remove_zeros();

        this->sign = this->is_zero() ? 1 : this->sign * a.sign;
        this->precision = precision;

        return *this;
    }

    BigNum operator/(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result /= b;
        return result;
    }

    bool BigNum::operator<(const BigNum &v) const
    {
        if (sign != v.sign)
            return sign < v.sign;
        if (digits.size() != v.digits.size())
            return digits.size() * sign < v.digits.size() * v.sign;
        for (size_t i = digits.size() - 1; i >= 0; --i)
            if (digits[i] != v.digits[i])
                return digits[i] * sign < v.digits[i] * sign;
        return false;
    }

    bool BigNum::operator>(const BigNum &other) const { return other < *this; }

    bool BigNum::operator<=(const BigNum &other) const { return !(other < *this); }

    bool BigNum::operator>=(const BigNum &other) const { return !(*this < other); }

    bool BigNum::operator==(const BigNum &other) const
    {
        if (this->digits.size() != other.digits.size() || this->precision != other.precision || this->sign != other.sign)
            return false;
        for (size_t i = 0; i < this->digits.size(); ++i)
        {
            if (this->digits[i] != other.digits[i])
                return false;
        }
        return true;
    }
    bool BigNum::operator!=(const BigNum &other) const { return !(*this == other); }

    bool BigNum::is_zero() const
    {
        return digits.size() == 1 && digits[0] == 0;
    }

    void BigNum::remove_zeros()
    {
        size_t n = std::max(static_cast<size_t>(1), precision);
        while (digits.size() > n && !digits.back())
        {
            digits.pop_back();
        }
        size_t i = 0;
        for (i; i < digits.size() && !digits[i]; ++i)
            ;
        digits.erase(digits.begin(), digits.begin() + i);
        precision -= i;
        if (digits.empty() || digits.size() == 1 && !digits[0])
        {
            digits.resize(1);
            precision = 1;
            sign = 0;
        }
    }

    std::ostream &operator<<(std::ostream &out, const BigNum &a)
    {
        if (a.sign < 0)
        {
            out << '-';
        }
        if (a.precision > 0)
        {
            size_t i = 0;
            size_t e = a.precision;
            while (i < a.digits.size() && i < e)
            {
                out << a.digits[i++];
            }
            while (i < e)
            {
                out << "0";
                ++i;
            }
            if (i < a.digits.size())
            {
                out << ".";
                while (i < a.digits.size())
                {
                    out << a.digits[i++];
                }
            }
        }
        else if (!a.precision)
        {
            out << "0.";
            for (size_t i = 0; i < a.digits.size(); ++i)
                out << a.digits[i];
        }
        else
        {
            out << "0.";
            size_t exp = -a.precision;
            for (size_t i = 0; i < exp; ++i)
            {
                out << "0";
            }
            for (size_t i = 0; i < a.digits.size(); ++i)
            {
                out << a.digits[i];
            }
        }
        return out;
    }

    // BigNum operator"" _bn(const char *str, std::size_t)
    // {
    //     return BigNum(str);
    // }

    // BigNum operator"" _bn(long double num)
    // {
    //     return BigNum(num);
    // }
} // namespace LongArithmetics