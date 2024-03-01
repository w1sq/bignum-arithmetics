#include "long_arithmetics.hpp"
#include <iostream>

int main()
{
    using namespace LongArithmetics;

    BigNum a("10");
    BigNum b("3");

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';

    BigNum sum = a + b;
    std::cout << "a + b = " << sum << '\n';

    BigNum diff = a - b;
    std::cout << "a - b = " << diff << '\n';

    BigNum prod = a * b;
    std::cout << "a * b = " << prod << '\n';

    BigNum quot = a / b;
    std::cout << "a / b = " << quot << '\n';

    return 0;
}