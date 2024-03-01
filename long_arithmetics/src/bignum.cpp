#include <long_arithmetics.hpp>

#include <iomanip>
#include <deque>

namespace LongArithmetics
{
    BigNum::BigNum()
    {
        sign = 0;
        digits = {0};
        fdigit = 0;
    }

    BigNum::BigNum(const std::string &number)
    {
        size_t i = 0;
        sign = number[0] == '-' ? ++i : 0;
        fdigit = number.size() - i;
        digits.reserve(fdigit);
        while (i < number.size())
        {
            if (number[i] == '.')
            {
                fdigit = sign ? i - 1 : i;
            }
            else if (isdigit(number[i]))
            {
                digits.push_back(number[i] - '0');
            }
            ++i;
        }
        remove_zeros();
    }

    BigNum::BigNum(long double d)
    {
        std::ostringstream os;
        os << std::setprecision(15) << d;
        *this = BigNum(os.str());
    }

    BigNum::BigNum(const BigNum &other)
        : digits(other.digits), sign(other.sign), fdigit(other.fdigit) {}

    BigNum::BigNum(BigNum &&other)
        : digits(move(other.digits)), sign(other.sign), fdigit(other.fdigit)
    {
        other.sign = 0;
        other.fdigit = 1;
    }

    BigNum operator""_bn(long double number)
    {
        return BigNum(number);
    }

    BigNum BigNum::operator-() const
    {
        BigNum result = *this;
        result.sign = !result.sign;
        return result;
    }

    bool BigNum::operator<(const BigNum &other) const
    {
        if (sign != other.sign)
            return sign > other.sign;
        if (fdigit != other.fdigit)
            return (fdigit < other.fdigit) ^ sign;
        return (digits < other.digits) ^ sign;
    }

    bool BigNum::operator>(const BigNum &other) const { return other < *this; }

    bool BigNum::operator<=(const BigNum &other) const { return !(other < *this); }

    bool BigNum::operator>=(const BigNum &other) const { return !(*this < other); }

    bool BigNum::operator==(const BigNum &other) const
    {
        if (digits.size() != other.digits.size() || fdigit != other.fdigit || sign != other.sign)
            return false;
        for (size_t i = 0; i < digits.size(); ++i)
        {
            if (digits[i] != other.digits[i])
                return false;
        }
        return true;
    }

    bool BigNum::operator!=(const BigNum &other) const { return !(*this == other); }

    BigNum &BigNum::operator=(const BigNum &other)
    {
        if (this == &other)
        {
            return *this;
        }
        sign = other.sign;
        fdigit = other.fdigit;
        digits = other.digits;
        return *this;
    }

    BigNum &BigNum::operator=(BigNum &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        sign = other.sign;
        fdigit = other.fdigit;
        digits = move(other.digits);
        other.sign = 0;
        other.fdigit = 1;
        return *this;
    }

    BigNum &BigNum::operator+=(const BigNum &a)
    {
        if (sign == a.sign)
        {
            auto afdigit = a.fdigit;
            auto max_fdigit = std::max(fdigit, afdigit);
            std::deque<int> digits_deq(digits.begin(), digits.end());
            std::deque<int> a_digits_deq(a.digits.begin(), a.digits.end());
            while (fdigit != max_fdigit)
            {
                digits_deq.push_front(0);
                ++fdigit;
            }
            while (afdigit != max_fdigit)
            {
                a_digits_deq.push_front(0);
                ++afdigit;
            }
            size_t size = std::max(digits_deq.size(), a_digits_deq.size());
            while (digits_deq.size() != size)
            {
                digits_deq.push_back(0);
            }
            while (a_digits_deq.size() != size)
            {
                a_digits_deq.push_back(0);
            }
            size_t len = 1 + size;
            fdigit = max_fdigit + 1;
            digits = std::vector<int>(len, 0);
            for (size_t i = 0; i < size; ++i)
            {
                digits[i + 1] = digits_deq[i] + a_digits_deq[i];
            }
            for (size_t i = len - 1; i > 0; --i)
            {
                digits[i - 1] += digits[i] / 10;
                digits[i] %= 10;
            }
            remove_zeros();
        }
        else if (sign)
        {
            *this = a - (-(*this));
        }
        else
        {
            *this -= (-a);
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
        if (!sign && !a.sign)
        {
            bool first_is_bigger = *this > a;
            auto fdigit1 = first_is_bigger ? fdigit : a.fdigit;
            auto fdigit2 = first_is_bigger ? a.fdigit : fdigit;
            auto max_fdigit = std::max(fdigit1, fdigit2);
            std::deque<int> digits_deq, a_digits_deq;
            if (first_is_bigger)
            {
                digits_deq = {digits.begin(), digits.end()};
                a_digits_deq = {a.digits.begin(), a.digits.end()};
            }
            else
            {
                digits_deq = {a.digits.begin(), a.digits.end()};
                a_digits_deq = {digits.begin(), digits.end()};
            }
            while (fdigit1 != max_fdigit)
            {
                digits_deq.push_front(0);
                ++fdigit1;
            }
            while (fdigit2 != max_fdigit)
            {
                a_digits_deq.push_front(0);
                ++fdigit2;
            }
            size_t size = std::max(digits_deq.size(), a_digits_deq.size());
            while (digits_deq.size() != size)
            {
                digits_deq.push_back(0);
            }
            while (a_digits_deq.size() != size)
            {
                a_digits_deq.push_back(0);
            }
            size_t len = 1 + size;
            sign = first_is_bigger ? 0 : 1;
            fdigit = max_fdigit + 1;
            digits = std::vector<int>(len, 0);
            for (size_t i = 0; i < size; ++i)
            {
                digits[i + 1] = digits_deq[i] - a_digits_deq[i];
            }
            for (size_t i = len - 1; i; --i)
            {
                if (digits[i] < 0)
                {
                    digits[i] += 10;
                    --digits[i - 1];
                }
            }
            remove_zeros();
        }
        else if (sign && a.sign)
        {
            *this = (-a) - (-(*this));
        }
        else
        {
            *this += (-a);
        }
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
        sign = sign ^ a.sign;
        fdigit = fdigit + a.fdigit;
        if ((digits.size() + a.digits.size()) < SIMPLE_LIMIT)
        {
            digits = simple_mul(digits, a.digits);
        }
        else
        {
            digits = karatsuba_mul(digits, a.digits);
        }
        for (size_t i = digits.size() - 1; i > 0; --i)
        {
            digits[i - 1] += digits[i] / 10;
            digits[i] %= 10;
        }
        remove_zeros();
        return *this;
    }
    std::vector<int> simple_mul(const std::vector<int> &lhs, const std::vector<int> &rhs)
    {
        std::vector<int> res(lhs.size() + rhs.size());
        for (size_t i = 0; i < lhs.size(); ++i)
        {
            for (size_t j = 0; j < rhs.size(); ++j)
            {
                res[i + j + 1] += lhs[i] * rhs[j];
            }
        }
        return res;
    }

    std::vector<int> karatsuba_mul(const std::vector<int> &lhs, const std::vector<int> &rhs)
    {
        size_t old_size = std::max(lhs.size(), rhs.size());
        std::vector<int> result;
        if ((old_size << 1) < SIMPLE_LIMIT)
        {
            result = simple_mul(lhs, rhs);
            return result;
        }
        size_t lz = 0;
        size_t rz = 0;
        size_t new_size = old_size;
        while (new_size & (new_size - 1))
        {
            ++new_size;
        }
        std::vector<int> left(new_size, 0), right(new_size, 0);
        for (size_t i = 0; i < lhs.size(); ++i)
        {
            left[i + new_size - lhs.size()] = lhs[i];
        }
        for (size_t i = 0; i < rhs.size(); ++i)
        {
            right[i + new_size - rhs.size()] = rhs[i];
        }
        size_t k = new_size >> 1;
        std::vector<int> a{left.begin(), left.begin() + k};
        std::vector<int> b{left.begin() + k, left.end()};
        std::vector<int> c{right.begin(), right.begin() + k};
        std::vector<int> d{right.begin() + k, right.end()};
        std::vector<int> ac = karatsuba_mul(a, c);
        std::vector<int> bd = karatsuba_mul(b, d);
        for (size_t i = 0; i < k; ++i)
        {
            a[i] += b[i];
            c[i] += d[i];
        }
        a = karatsuba_mul(a, c);
        for (size_t i = 0; i < new_size; ++i)
        {
            a[i] -= ac[i] + bd[i];
        }
        result.resize(2 * new_size);
        for (size_t i = 0; i < new_size; ++i)
        {
            result[new_size + i] = bd[i];
        }
        for (size_t i = 0; i < new_size; ++i)
        {
            result[i] = ac[i];
        }
        for (size_t i = k; i < new_size + k; ++i)
        {
            result[i] += a[i - k];
        }
        result.erase(result.begin(), result.begin() + new_size - lhs.size() + new_size - rhs.size());
        return result;
    }

    BigNum
    operator*(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result *= b;
        return result;
    }

    BigNum BigNum::inverse() const
    {
        BigNum inversed;
        if (digits.size() == 1 && !digits[0])
        {
            throw std::invalid_argument("Zero division");
        }
        BigNum x = *this;
        x.sign = 0;
        BigNum one("1");
        inversed.sign = sign;
        inversed.fdigit = 1;
        inversed.digits.clear();
        while (x < one)
        {
            ++x.fdigit;
            ++inversed.fdigit;
        }
        while (one < x)
        {
            one.fdigit++;
        }
        inversed.fdigit -= one.fdigit - 1;
        size_t totalNumbers = divDigits + std::max(0, inversed.fdigit);
        size_t numbers = 0;
        do
        {
            int div = 0;
            while (one >= x)
            {
                ++div;
                one -= x;
            }
            ++one.fdigit;
            one.remove_zeros();
            inversed.digits.push_back(div);
            ++numbers;
        } while (!(one.is_zero()) && numbers < totalNumbers);
        return inversed;
    }

    BigNum &BigNum::operator/=(const BigNum &a)
    {
        if (is_zero())
            return *this;
        *this *= a.inverse();
        size_t intPart = std::max(0, a.fdigit);
        if (intPart > digits.size() - 1)
            return *this;
        size_t i = digits.size() - 1 - intPart;
        size_t n = std::max(0, fdigit);
        if (i > n && digits[i] == 9)
        {
            while (i > n && digits[i] == 9)
            {
                i--;
            }
            if (digits[i] == 9)
            {
                digits.erase(digits.begin() + n, digits.end());
                *this += BigNum(sign ? -1 : 1);
            }
            else
            {
                digits.erase(digits.begin() + i + 1, digits.end());
                digits[i]++;
            }
        }
        return *this;
    }

    BigNum operator/(const BigNum &a, const BigNum &b)
    {
        BigNum result = a;
        result /= b;
        return result;
    }

    bool BigNum::is_zero() const
    {
        return (digits.size() == 1 && !digits[0]);
    }

    void BigNum::remove_zeros()
    {
        size_t n = std::max(1, fdigit);
        while (digits.size() > n && !digits.back())
        {
            digits.pop_back();
        }
        size_t i = 0;
        for (i; i < digits.size() && !digits[i]; ++i)
            ;
        digits.erase(digits.begin(), digits.begin() + i);
        fdigit -= i;
        if (digits.empty() || digits.size() == 1 && !digits[0])
        {
            digits.resize(1);
            fdigit = 1;
            sign = 0;
        }
    }

    std::ostream &operator<<(std::ostream &out, const BigNum &a)
    {
        if (a.sign)
            out << '-';
        if (a.fdigit > 0)
        {
            size_t i = 0;
            size_t e = a.fdigit;
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
        else if (!a.fdigit)
        {
            out << "0.";
            for (size_t i = 0; i < a.digits.size(); ++i)
                out << a.digits[i];
        }
        else
        {
            out << "0.";
            size_t min_a_fdigit = -a.fdigit;
            for (size_t i = 0; i < min_a_fdigit; ++i)
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

    std::string to_string(const BigNum &number)
    {
        std::stringstream ss;
        ss << number;
        return ss.str();
    }
} // namespace LongArithmetics