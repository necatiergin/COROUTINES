#include <coroutine>
#include <iostream>

struct DirectAwaiter {
    int mval;
    bool await_ready() noexcept 
    {
        std::cout << "[DirectAwaiter] await_ready (mval=" << mval << ")\n";
        return false; // suspend yolu izlenecek
    }
    
    bool await_suspend(std::coroutine_handle<> h) noexcept 
    {
        std::cout << "[DirectAwaiter] await_suspend -> returning false (don't suspend)\n";
        return false; 
    }

    void await_resume() noexcept 
    {
        std::cout << "[DirectAwaiter] await_resume\n";
    }
};


struct Task {
    struct promise_type {
        Task get_return_object() noexcept { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }

        
        DirectAwaiter await_transform(int x) 
        {
            std::cout << "[Task::promise_type::await_transform] called with " << x 
            << " -> returning DirectAwaiter\n";
            return DirectAwaiter{ x };
        }
    };
};

Task foo() 
{
    std::cout << "foo called\n";
    co_await 42; 
    std::cout << "foo is running after co_await\n";
    co_return;
}

int main() 
{
    foo();
}
