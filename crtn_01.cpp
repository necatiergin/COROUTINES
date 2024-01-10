#include <iostream>
#include <coroutine>

class CrtTask {
public:
    struct promise_type;    
    using CoroutineHandle = std::coroutine_handle<promise_type>;
private:
    CoroutineHandle m_handle;
public:
    CrtTask(auto h): m_handle{ h } {}
    ~CrtTask() 
    {
        if (m_handle) {
            m_handle.destroy();
        }
    }
    
    CrtTask(const CrtTask&) = delete;
    CrtTask& operator=(const CrtTask&) = delete;

    bool resume() const 
    {
        if (!m_handle || m_handle.done()) {
            return false;
        }
        m_handle.resume();
        return !m_handle.done();
    }
};


struct CrtTask::promise_type {
    CrtTask get_return_object() 
    {
        return CrtTask{ CoroutineHandle::from_promise(*this) };
    }
    
    auto initial_suspend() 
    {
        return std::suspend_always{};
    }

    void unhandled_exception() 
    {
        std::terminate();
    }

    void return_void() {}

    auto final_suspend() noexcept 
    {
        return std::suspend_always{};  
    }
};


CrtTask crfunc(int n)
{
  std::cout << "         crfunc " << n << " start\n";

  for (int i{ 1 }; i <= n; ++i) {
      std::cout << "         crfunc " << i << '/' << n << '\n';

    co_await std::suspend_always();    
  }

  std::cout << "         crfunc " << n << " end\n";
}


int main()
{
    auto ctask = crfunc(3); 
    std::cout << "crfunc() started\n";
    
    while (ctask.resume()) { 
        std::cout << "crfunc() suspended\n";
    }
    std::cout << "crfunc() done\n";
}
