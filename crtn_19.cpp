#include <generator> //C++23
#include <utility>

std::generator<int> fib(int n) 
{
    int x{ 0 }, y{ 1 };

    while (n--)
         co_yield std::exchange(x, std::exchange(y, x + y));
}

int main() 
{
    for (int i : fib(10)) {
        std::cout << i;
    }
}
