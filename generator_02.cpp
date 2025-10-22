#include <generator>
#include <iostream>
#include <memory>

std::generator<std::unique_ptr<int>> make_unique_stream(int n)
{
    for (int i = 0; i < n; ++i)
        co_yield std::make_unique<int>(i); // can yield move-only types
}

int main()
{
    for (auto ptr : make_unique_stream(3))
        std::cout << *ptr << ' '; // 0 1 2
}
