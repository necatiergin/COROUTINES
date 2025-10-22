#include <coroutine>
#include <iostream>


struct Wrapper {
    int mval;
};

struct IndirectAwaiter {
    Wrapper w;
    bool await_ready() noexcept 
    {
        std::cout << "[IndirectAwaiter] await_ready (mval=" << w.mval << ")\n";
        return false;
    }

    bool await_suspend(std::coroutine_handle<>) noexcept 
    {
        std::cout << "[IndirectAwaiter] await_suspend -> returning false\n";
        return false;
    }
    void await_resume() noexcept 
    {
        std::cout << "[IndirectAwaiter] await_resume\n";
    }
};

IndirectAwaiter operator co_await(Wrapper w) 
{
    std::cout << "[free operator co_await] called for Wrapper(value = " << w.mval<< ")\n";
    return IndirectAwaiter{ w };
}

struct TaskIndirect {
    struct promise_type {
        TaskIndirect get_return_object() noexcept 
        {
            return {}; 
        }

        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }

        // Burada await_transform bir Wrapper döndürüyor (awaiter değil)
        Wrapper await_transform(int v) 
        {
            std::cout << "[TaskIndirect::promise_type::await_transform] called with " 
                << v << " -> returning Wrapper\n";
            return Wrapper{ v };
        }
    };
};

TaskIndirect bar() 
{
    std::cout << ">> bar: before co_await\n";
    co_await 7; 
    std::cout << ">> bar: after co_await\n";
    co_return;
}

int main() 
{
    std::cout << "=== Running bar ===\n";
    bar();
    std::cout << "=== bar finished ===\n\n";
}
