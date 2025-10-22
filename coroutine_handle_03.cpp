#include <coroutine>

struct Task {
    struct promise_type {
        auto get_return_object() 
        {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) }; 
        }
        
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; } 
        void return_void() noexcept {}
        void unhandled_exception() noexcept {}
    };

    std::coroutine_handle<promise_type> handle;
    Task(std::coroutine_handle<promise_type> h_) : handle(h_) {}
    
    ~Task() 
    {
        if (handle) 
            handle.destroy(); 
    } 
    
    Task(Task&& o) : handle(o.handle) { o.handle = {}; }
    Task(const Task&) = delete;
    // ...
};
