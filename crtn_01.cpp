//minimal coroutine 

#include <coroutine>
#include <iostream>

class ReturnType{
public:
    class promise_type {
    public:
        ReturnType get_return_object()
        {
            return {};
        }
        
        std::suspend_never initial_suspend() 
        {
            return {};
        }
        
        void return_void() {}
        
        void unhandled_exception() {}
        
        std::suspend_always final_suspend() noexcept 
        {
            return {};
        }
    };
};

ReturnType crfunc() 
{
    std::cout << "hello coroutine\n";
    
    co_return;
}

int main() 
{
    ReturnType demo = crfunc();
}
