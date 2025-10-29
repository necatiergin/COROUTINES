#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>   

template<typename T>
class Negenerator {
public:
    struct promise_type {
        T current_value;
        std::exception_ptr exception;

        Negenerator get_return_object() 
        {
            return Negenerator {
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(T value) 
        {
            current_value = std::move(value);
            return {};
        }

        void return_void() {}

        void unhandled_exception() 
        {
            exception = std::current_exception(); 
        }
    };

    // Iterator for range-based for
    struct iterator {
        using coro_handle = std::coroutine_handle<promise_type>;

        coro_handle handle = nullptr;

        iterator() = default;
        explicit iterator(coro_handle h) : handle(h) 
        {
            // When created for begin(), coroutine is suspended at initial_suspend.
            // We need to resume to reach the first co_yield (if any).
            if (handle) {
                // resume until the first value or until coroutine finishes
                handle.resume();
                // if exception occurred during initial run, rethrow here
                if (handle.done()) {
                    // finished without yielding; any exception captured in promise
                    auto& p = handle.promise();
                    if (p.exception) 
                        std::rethrow_exception(p.exception);
                }
            }
        }

        iterator& operator++() 
        {
            // resume to produce next value (or finish)
            handle.resume();
            if (handle.done()) {
                // if coroutine finished and stored exception, rethrow now
                auto& p = handle.promise();
                if (p.exception) 
                    std::rethrow_exception(p.exception);
            }
            return *this;
        }

        const T& operator*() const 
        {
            return handle.promise().current_value;
        }

        bool operator==(std::default_sentinel_t) const 
        {
            return !handle || handle.done();
        }

        bool operator!=(std::default_sentinel_t s) const 
        {
            return !(*this == s);
        }
    };

    // generator is move-only
    Negenerator(Negenerator&& other) noexcept : handle_(other.handle_) { other.handle_ = {}; }
    Negenerator& operator=(Negenerator&& other) noexcept 
    {
        if (this != &other) {
            destroy();
            handle_ = other.handle_;
            other.handle_ = {};
        }
        return *this;
    }

    // non-copyable
    Negenerator(const Negenerator&) = delete;
    Negenerator& operator=(const Negenerator&) = delete;

    ~Negenerator() 
    {
        destroy();
    }

    iterator begin() 
    {
        if (!handle_) 
            return iterator{}; // empty generator
        return iterator{ handle_ };
    }

    std::default_sentinel_t end() noexcept 
    {
        return {};
    }

private:
    using coro_handle = std::coroutine_handle<promise_type>;

    explicit Negenerator(coro_handle h) : handle_(h) {}

    void destroy() 
    {
        if (handle_) {
            handle_.destroy();
            handle_ = nullptr;
        }
    }

    coro_handle handle_;
};


Negenerator<int> generate_numbers(int start, int end) 
{
    for (int i = start; i <= end; ++i) {
        co_yield i;
    }
}

Negenerator<unsigned long long> fibonacci(size_t n) 
{
    unsigned long long a = 0, b = 1;
    for (size_t i = 0; i < n; ++i) {
        co_yield a;
        a = std::exchange(b, a + b);
    }
}

int main() 
{
    std::cout << "Numbers 1..9: ";
    for (auto v : generate_numbers(1, 9)) {
        std::cout << v << ' ';
    }
    std::cout << '\n';

    std::cout << "First 15 Fibonacci: ";
    
    for (auto v : fibonacci(15)) {
        std::cout << v << ' ';
    }
   
    std::cout << '\n';
    
}
