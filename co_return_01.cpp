#include <coroutine>
#include <iostream>
#include <string>

struct Negenerator {
    struct promise_type {
        std::string m_val;
        Negenerator get_return_object() 
        {
            return Negenerator{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() { std::terminate(); }

        void return_value(std::string value) 
        {
            m_val = std::move(value);
        }
    };

    std::coroutine_handle<promise_type> handle;
    explicit Negenerator(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~Negenerator() 
    {
        if (handle) 
            handle.destroy(); 
    }
};

Negenerator foo() 
{
    co_return "hello coroutine";
}

int main() 
{
    auto gen = foo();
    
    std::cout << gen.handle.promise().m_val << '\n';
}
