/*
* 第三章中实现的线程安全栈
*
*
*/

#include <iostream>
#include <memory>
#include <mutex>
#include <exception>
#include <thread>
#include <stack>
#include <algorithm>

struct empty_stack:public std::exception
{
    const char* what() const noexcept //不抛出异常
    {
        return "empty stack";
    }
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other)   //允许拷贝
    {
        std::lock_guard<std::mutex> guard(other.m_mutex);
        m_st = other.m_st;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;  //不允许赋值

    void push(const T& val)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_st.push(val);
    }

    void pop(T& val)
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        if(m_st.empty()) throw empty_stack();
        val = m_st.top();
        m_st.pop();
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        if(m_st.empty()) throw empty_stack();
        std::shared_ptr<T> p(std::make_shared<T>(m_st.top()));
        m_st.pop();
        return p;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_st.empty();
    }

private:
    std::mutex    m_mutex;
    std::stack<T> m_st;
};

int main()
{
    threadsafe_stack<int> st;
    auto task = [&st](){ 
        while(1)
        {
            try
            {
                auto p = st.pop();
                std::cout<<*p<<std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        std::cout<<"thread th end"<<std::endl;
    };

    std::thread th(task);   //task对象被复制到th线程的内部，被线程调用

    int limit = 1000;
    int i = 0;
    while(i++ < limit)
    {
        st.push(i);
    }

    th.join();

    return 0;
}