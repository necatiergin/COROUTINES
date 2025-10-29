#include <coroutine>
#include <generator>
#include <stdexcept>
#include <iostream>

std::generator<int> foo(int first, int last)
{
	if (last < first)
		throw std::invalid_argument{ "error from foo : bad_argument\n" };

	for (int i = first; i < last; ++i) {
		co_yield i;
	}
}

int main()
{
	try {
		for (auto val : foo(9, 2))
		//for (auto val : foo(2, 9))
			std::cout << val << ' ';
	}
	catch (const std::exception& ex) {
		std::cout << "exception caught: " << ex.what() << '\n';
	}
}
