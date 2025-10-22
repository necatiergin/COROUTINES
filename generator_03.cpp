#include <generator>
#include <iostream>
#include <vector>

std::generator<int&> iterate_refs(std::vector<int>& vec)
{
    for (auto& x : vec)
        co_yield x; // yields a reference
}

int main()
{
    std::vector<int> v{ 1, 2, 3 };
    
    for (int& x : iterate_refs(v))
        x *= 10;

    for (int x : v)
        std::cout << x << ' '; // 10 20 30
}
