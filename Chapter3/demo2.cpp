/*
* std::unique_ptr使用，比scoped_lock、lock_guard有额外开销
* std::unique_ptr支持移动，可以转移所有权，更加灵活
*
*/

#include <iostream>
#include <thread>
#include <mutex>

struct D
{
    int m_data;
};

class X
{
public:
    friend void swap(X& x1, X& x2);

private:
    std::mutex m_mutex;
    D m_d;
};

void swap(X& x1, X& x2)
{
    if(&x1 == &x2)
        return;

    std::unique_lock<std::mutex> lock_a(x1.m_mutex, std::defer_lock);  //std::defer_lock延迟加锁，先不上锁
    std::unique_lock<std::mutex> lock_b(x1.m_mutex, std::defer_lock);  //std::unique_ptr内部实现了lock()、unlock()、try_lock()，所以可以将其实例传给lock函数
    std::lock(lock_a, lock_b);  //在这里同时锁住2个
}

int main()
{
    return 0;
}