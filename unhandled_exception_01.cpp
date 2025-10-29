#include <coroutine>
#include <exception>
#include <iostream>
#include <memory>

template<typename T>
struct Task {
    struct promise_type {
        T m_val{};
        std::exception_ptr ex;

        Task get_return_object() 
        {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_value(const T& x) 
        {
            if (x == 0)
                throw std::runtime_error{ "error from promise_type::return_value()\n" };
            m_val = x;
        }

        void unhandled_exception() noexcept 
        {
            ex = std::current_exception();
        }
    };

    std::coroutine_handle<promise_type> h;

    explicit Task(std::coroutine_handle<promise_type> h) : h(h) {}
    ~Task() 
    {
        if (h) 
            h.destroy(); 
    }

    T get() 
    {
        if (h.promise().ex) 
            std::rethrow_exception(h.promise().ex);
        return h.promise().m_val;
    }
};

Task<int> foo()
{
    //co_return 0;
     co_return 75;
}

int main()
{
    auto task = foo();

    try {
        auto val = task.get();
        std::cout << "val = " << val << '\n';

    }
    catch (const std::exception& ex) {
        std::cout << "exception caught: " << ex.what() << '\n';
    }
}
