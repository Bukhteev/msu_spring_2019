#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

size_t N = 500000;
int i = 0;

std::mutex mut;
std::condition_variable cond;

void ping()
{
    while (i < 2 * N - 1)
    {
        std::unique_lock<std::mutex> lock(mut);
        while (i % 2 == 1)
            cond.wait(lock);
        std::cout << "ping\n";
        ++i;
        cond.notify_one();
    }
}

void pong() 
{
    while (i < 2 * N) 
    {
        std::unique_lock<std::mutex> lock(mut);
        while (i % 2 == 0)
            cond.wait(lock);
        std::cout << "pong\n";
        ++i;
        cond.notify_one();
    }
}

int main()
{
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();    
    return 0;
}
