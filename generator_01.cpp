#include <generator>
#include <iostream>

std::generator<int> range_inclusive(int first, int last)
{
    for (int i = first; i <= last; ++i)
        co_yield i; // yield the next value
}

int main()
{
    for (int x : range_inclusive(3, 7)) // lazy iteration
        std::cout << x << ' '; // 3 4 5 6 7
}
