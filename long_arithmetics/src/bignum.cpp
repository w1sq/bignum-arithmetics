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
        sign = 1;
        precision = 0;
        std::string integerPart, fractionalPart;

        size_t commaPos = number.find('.');
        if (commaPos != std::string::npos)
        {
            integerPart = number.substr(0, commaPos);
            fractionalPart = number.substr(commaPos + 1);
        }
        else
        {
            integerPart = number;
        }

        if (integerPart[0] == '-')
        {
            sign = -1;
            integerPart = integerPart.substr(1);
        }

        for (int i = integerPart.length(); i > 0; i -= base_digits)
        {
            if (i < base_digits)
                digits.push_back(std::stoi(integerPart.substr(0, i)));
            else
                digits.push_back(std::stoi(integerPart.substr(i - base_digits, base_digits)));
        }

        for (int i = 0; i < fractionalPart.length(); i += base_digits)
        {
            if (i + base_digits > fractionalPart.length())
                digits.push_back(std::stoi(fractionalPart.substr(i)));
            else
                digits.push_back(std::stoi(fractionalPart.substr(i, base_digits)));
        }
        precision = fractionalPart.length();
    }

    // BigNum::BigNum(long long num, size_t precision)
    // {
    //     std::ostringstream strs;
    //     strs << num;
    //     std::string str = strs.str();

    //     // Append zeros based on precision
    //     str.append(precision, '0');

    //     // Use the string constructor
    //     *this = BigNum(str);
    // }

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
        while (!digits.empty() && digits.back() == 0)
        {
            digits.pop_back();
        }
        if (digits.empty())
        {
            digits.push_back(0);
            sign = 1;
        }
    }

    std::ostream &operator<<(std::ostream &out, const BigNum &a)
    {
        if (a.sign < 0)
            out << '-';

        size_t integerPartSize = a.digits.size() - a.precision;

        // Print integer part
        for (size_t i = 0; i < integerPartSize; ++i)
            out << a.digits[i];

        if (a.precision > 0)
        {
            out << '.';

            // Print fractional part
            for (size_t i = integerPartSize; i < a.digits.size(); ++i)
                out << a.digits[i];
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