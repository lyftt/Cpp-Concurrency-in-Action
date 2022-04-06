/*
* std::lock的使用
* std::scoped_lock的使用
*
*/

#include <iostream>
#include <mutex>
#include <thread>

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

    std::lock(x1.m_mutex, x2.m_mutex);  //同时锁住两个锁，要么全部所住，要么全都不锁住，可以避免死锁, 这里可能抛出异常
    std::lock_guard<std::mutex> g1(x1.m_mutex, std::adopt_lock);   //std::adopt_lock表示已经锁上了，不要在构造函数里加锁
    std::lock_guard<std::mutex> g2(x2.m_mutex, std::adopt_lock);
    //std::scoped_lock guard(x1.m_mutex, x2.m_mutex);   //c++17的scoped_lock和lock_guard等价，但是socped_lock支持多个锁同时上锁，且c++17支持类模板参数推导，不需要再在<>给出具体模板参数
                                                        //可以依据传入构造函数的参数对象自动匹配，推导出正确的类型
}

int main()
{
    return 0;
}