#include <iostream>
#include <iomanip>
#include <vector>

constexpr int base = 1000'000'000;
constexpr int base_digits = 9;

struct BigNum
{
    explicit BigNum(long long v = 0) { *this = v; }

    explicit BigNum(const std::string &s) { read_from_str(s); }

    BigNum &operator=(long long v)
    {
        sign = v < 0 ? -1 : 1;
        v *= sign;
        digits.clear();
        for (; v > 0; v = v / base)
            digits.push_back((int)(v % base));
        return *this;
    }

    friend BigNum operator-(BigNum v)
    {
        if (!v.digits.empty())
            v.sign = -v.sign;
        return v;
    }

    BigNum &operator+=(const BigNum &other)
    {
        if (sign == other.sign)
        {
            for (size_t i = 0, carry = 0; i < other.digits.size() || carry; ++i)
            {
                if (i == digits.size())
                    digits.push_back(0);
                digits[i] += carry + (i < other.digits.size() ? other.digits[i] : 0);
                carry = digits[i] >= base;
                if (carry)
                    digits[i] -= base;
            }
        }
        else if (!other.is_zero())
        {
            *this -= -other;
        }
        return *this;
    }

    friend BigNum operator+(BigNum a, const BigNum &b)
    {
        a += b;
        return a;
    }

    BigNum &operator-=(const BigNum &other)
    {
        if (sign == other.sign)
        {
            if ((sign == 1 && *this >= other) || (sign == -1 && *this <= other))
            {
                for (size_t i = 0, carry = 0; i < other.digits.size() || carry; ++i)
                {
                    digits[i] -= carry + (i < other.digits.size() ? other.digits[i] : 0);
                    carry = digits[i] < 0;
                    if (carry)
                        digits[i] += base;
                }
                remove_zeros();
            }
            else
            {
                *this = other - *this;
                this->sign = -this->sign;
            }
        }
        else
        {
            *this += -other;
        }
        return *this;
    }

    friend BigNum operator-(BigNum a, const BigNum &b)
    {
        a -= b;
        return a;
    }

    BigNum &operator*=(int v)
    {
        if (v < 0)
            sign = -sign, v = -v;
        for (size_t i = 0, carry = 0; i < digits.size() || carry; ++i)
        {
            if (i == digits.size())
                digits.push_back(0);
            long long cur = (long long)digits[i] * v + carry;
            carry = (int)(cur / base);
            digits[i] = (int)(cur % base);
        }
        remove_zeros();
        return *this;
    }

    BigNum operator*(int v) const { return BigNum(*this) *= v; }

    bool operator<(const BigNum &v) const
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

    bool operator>(const BigNum &v) const { return v < *this; }

    bool operator<=(const BigNum &v) const { return !(v < *this); }

    bool operator>=(const BigNum &v) const { return !(*this < v); }

    bool operator==(const BigNum &v) const { return sign == v.sign && digits == v.digits; }

    bool operator!=(const BigNum &v) const { return !(*this == v); }

    friend std::ostream &operator<<(std::ostream &out, const BigNum &num);

    bool is_zero() const
    {
        for (size_t i = 0; i < digits.size(); ++i)
        {
            if (digits[i] != 0)
            {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<int> digits;
    size_t precision;
    int sign; // 1 for >=0, -1 for <0

    void read_from_str(const std::string &s)
    {
        sign = 1;
        digits.clear();
        size_t pos = 0;
        while (pos < s.size() && (s[pos] == '-' || s[pos] == '+'))
        {
            if (s[pos] == '-')
                sign = -sign;
            ++pos;
        }
        for (size_t i = s.size() - 1; i >= pos; i -= base_digits)
        {
            int x = 0;
            for (size_t j = std::max(pos, i - base_digits + 1); j <= i; j++)
                x = x * 10 + s[j] - '0';
            digits.push_back(x);
        }
        remove_zeros();
    }

    void remove_zeros()
    {
        while (!digits.empty() && digits.back() == 0)
            digits.pop_back();
        if (digits.empty())
        {
            sign = 1;
        }
    }
};

std::ostream &operator<<(std::ostream &out, const BigNum &v)
{
    if (v.sign == -1)
        out << '-';
    out << (v.digits.empty() ? 0 : v.digits.back());
    for (size_t i = v.digits.size() - 2; i >= 0; --i)
        out << std::setw(base_digits) << std::setfill('0') << v.digits[i];
    return out;
}

int main()
{
    BigNum x("120");
    BigNum y("0");
    x += y;
    std::cout << x << std::endl;
}