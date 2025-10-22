#include <coroutine>

struct Task {
    struct promise_type {
        Task get_return_object()
        {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        // ...
    };

    // ...
    Task(std::coroutine_handle<promise_type>);
};
