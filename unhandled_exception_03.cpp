#include <optional>
#include <coroutine>
#include <stdexcept>
#include <iostream>

struct Generator {
    struct promise_type {
        std::optional<int> current;
        std::exception_ptr ex;

        Generator get_return_object() {
            return Generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value(int v) noexcept 
        {
            current = v;
            return {};
        }

        void return_void() noexcept {}

        void unhandled_exception() noexcept
        {
            ex = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Generator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Generator() 
    {
        if (handle)
            handle.destroy(); }

    bool next() 
    {
        if (handle.done()) {
            if (handle.promise().ex) 
                std::rethrow_exception(handle.promise().ex);
            return false;
        }
       
        handle.resume();
        
        if (handle.promise().ex) 
            std::rethrow_exception(handle.promise().ex);
        
        return !handle.done();
    }

    int current() const 
    {
        return *handle.promise().current;
    }
};

Generator foo() 
{
    co_yield 1;
    co_yield 2;
    throw std::runtime_error("exception after yields");
    co_yield 3; // unreachable
}


int main() 
{
    try {
        auto g = foo();
        while (g.next()) {
            std::cout << "got: " << g.current() << '\n';
        }
    }
    catch (const std::exception& e) {
        std::cout << "generator threw: " << e.what() << '\n';
    }
}
