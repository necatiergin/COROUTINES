#include <coroutine>
#include <random>
#include <iostream>
#include <exception>

class Generator {
public:
	struct promise_type {
		auto get_return_object()
		{
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}

		std::size_t m_value{};
		std::size_t m_count{};
		auto yield_value(std::size_t x)
		{
			m_value = x;
			++m_count;
			return std::suspend_always{};
		}

		void return_value(auto val)
		{
			m_count = val;
		}

		auto initial_suspend()
		{
			return std::suspend_always{};
		}

		auto final_suspend()noexcept
		{
			return std::suspend_always{};
		}
		
		void unhandled_exception()noexcept
		{
			std::terminate();
		}

	};
	Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {}
	Generator(const Generator&) = delete;
	Generator& operator=(const Generator&) = delete;

	~Generator()
	{
		if (m_handle)
			m_handle.destroy();
	}

	auto get_value() const
	{
		return m_handle.promise().m_value;
	}

	bool resume()const
	{
		if (!m_handle || m_handle.done())
			return false;
		m_handle.resume();

		return !m_handle.done();
	}

	auto get_count()const
	{
		return m_handle.promise().m_count;
	}

private:
	std::coroutine_handle<promise_type> m_handle{};
};

Generator func()
{
	static std::mt19937 eng;
	std::size_t cnt{};
	for (int i = 0; ; ++i) {
		auto val = eng();
		co_yield val;
		++cnt;
		if (val % 19 == 0)
			co_return cnt;
		
	}
}


int main()
{
	auto gen = func();

	while (gen.resume()) {
		std::cout << gen.get_value() << "\n";
	}

	std::cout << "total count = " << gen.get_count()  << "\n";

}
