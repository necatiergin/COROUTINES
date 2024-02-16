#include <coroutine>
#include <iostream>

struct ReturnObject {
    struct promise_type {
        promise_type() = default;
        ReturnObject get_return_object() { return {}; }
        
        //std::suspend_always initial_suspend() { return {}; }
        std::suspend_never initial_suspend() { return {}; }

        std::suspend_always final_suspend()noexcept { return {}; }
        void unhandled_exception() {}
        void return_void(){}
    };
};

ReturnObject cfunc() 
{
    std::cout << "necati\n";
    co_await std::suspend_always{}; 
    std::cout << "ergin";
}

int main() 
{
    auto ro = cfunc(); 
    //...
}
