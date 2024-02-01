#include <iostream>
#include <vector>

class BigNum{
    public:
        BigNum():digits(1,0){}

        BigNum(unsigned int x) {
            while (x > 0) {
                digits.push_back(x % 10);
                x /= 10;
            }
        }

        BigNum operator+(const BigNum& term) {
            BigNum sum;
            sum.digits.resize(std::max(this->digits.size(), term.digits.size()));
            for (int i = 0; i < sum.digits.size(); ++i) {
                sum.digits[i] = this->GetDigit(i) + term.GetDigit(i);
            }
            sum.FixDigits();
            return sum;
        }
        
        bool operator==(const BigNum& comp) {
            if (comp.digits.size() != this->digits.size()) {
                return false;
            }
            for (int i = 0; i < this->digits.size(); ++i) {
                if (comp.digits[i] != this->digits[i]) {
                    return false;
                }
            }
            return true;
        }

        friend std::ostream& operator<<(std::ostream& out, const BigNum& num);
    private:
        void FixDigits() {
            for (int i = 0; i < digits.size(); ++i) {
                int current_digit = digits[i] / 10;
                digits[i] %= 10;
                if (i == digits.size() - 1 && current_digit != 0) {
                    digits.push_back(current_digit);
                }
                else {
                    digits[i + 1] += current_digit;
                }
            }
        }

        int GetDigit(int idx) const {
            if (idx < 0)  {
                throw std::exception();
            }
            if (idx > digits.size() - 1) {
                return 0;
            }
            return digits[idx];
        }

        std::vector<int> digits;
};

std::ostream& operator<<(std::ostream& out, const BigNum& num) {
    for (int i = num.digits.size() - 1; i >= 0; --i) {
        out << num.digits[i];
    }
    return out;
}


int main(){
    BigNum x;
    std::cout << x << std::endl;

}