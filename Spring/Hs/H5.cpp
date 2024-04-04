#include <iostream>
#include <vector>

int main()
{
    f(static_cast<int> (5), static_cast<double> (5.4));    // f-T-U
    f(std::vector({1,2,3}), static_cast<int> (5));    // f-T*-int
    f("string", static_cast<int> (5));    // f-T*-int
    f(static_cast<double> (5.4), static_cast<int> (5), static_cast<int> (5));    // f-T-U-U
    f(static_cast<short> (5), static_cast<double> (5.4));    // f-short-U
    f(static_cast<short> (5), static_cast<short> (5));    // short-short
    f(static_cast<int> (5), static_cast<int> (5));    // int-int
}
