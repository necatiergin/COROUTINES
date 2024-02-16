#include <coroutine>
#include <iostream>

struct CoroFace {
    struct promise_type {
        promise_type()
        {
            std::cout << "promise type default ctor.\n";
        }
        
        CoroFace get_return_object() 
        { 
            std::cout << "promise_type::get_return_object()\n";

            return {}; 
        }

        std::suspend_never initial_suspend() 
        { 
            std::cout << "promise_type::initial_suspend()\n";
            return {}; 
        } 
        
        std::suspend_never final_suspend() noexcept 
        { 
            std::cout << "promise_type::final_suspend()\n";
            return {}; 
        }     

        void unhandled_exception() 
        {
            std::cout << "promise_type::unhandled exception()\n";
        }
        
        void return_void() 
        {
            std::cout << "promise_type::return_void()\n";
        }
    };
};

CoroFace foo() 
{
    std::cout << "coroutine code point [1]\n";
    //co_await std::suspend_always{};
    co_await std::suspend_never{};
    std::cout << "coroutine code point [2]\n";
 }

int main(void)
{
    foo();
}
