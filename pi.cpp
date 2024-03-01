#include <iostream>
#include <ctime>
#include <string>

#include <long_arithmetics.hpp>

using namespace LongArithmetics;

BigNum
calculate_pi(unsigned N)
{
    BigNum pi = 0.0_bn;
    BigNum x("16");
    for (int i = 0; i < N; i++)
    {
        BigNum magic = (BigNum("4") / BigNum(8 * i + 1));
        magic -= (BigNum("2") / BigNum(8 * i + 4));
        magic -= (BigNum("1") / BigNum(8 * i + 5));
        magic -= (BigNum("1") / BigNum(8 * i + 6));
        x /= BigNum("16");
        pi += magic * x;
    }
    return pi;
}

int main(void)
{
    unsigned N = 100;
    clock_t start = clock();
    BigNum pi = calculate_pi(N);
    clock_t end = clock();
    clock_t ms_time = (end - start) * 1000 / CLOCKS_PER_SEC;
    // std::cout << "Time: " << ms_time << "ms" << std::endl;
    BigNum ideal = BigNum("3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128");
    if (to_string(pi).substr(0, N) == to_string(ideal).substr(0, N) && ms_time < 1000)
        std::cout << "OK!" << std::endl;
    else
        std::cout << "Fault" << std::endl;
    return 0;
}