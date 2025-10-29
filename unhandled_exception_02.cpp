#include <coroutine>
#include <generator>
#include <stdexcept>
#include <iostream>


template <typename T>
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

    std::coroutine_handle<promise_type> handle;

    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}

    ~Task() 
    {
        if (handle) 
            handle.destroy(); 
    }

    T get() 
    {
        if (handle.promise().ex) 
            std::rethrow_exception(handle.promise().ex);
        return handle.promise().m_val;
    }
};

struct ThrowingAwaitable {
    bool await_ready() const noexcept 
    {
        return false;
    }
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    int await_resume() const 
    {
        std::cout << "await_resume called!\n";
        throw std::runtime_error("exception from await_resume");
        return 42;
    }
};

Task<int> foo()
{
    // burada await_resume() exception atacak
    int x = co_await ThrowingAwaitable{};
    co_return x;
}



int main()
{
    auto task = foo();

    task.handle.resume();
    try {
        std::cout << task.get() << '\n';

    }
    catch (const std::exception& ex) {
        std::cout << "exception caught: " << ex.what() << '\n';
    }

}
