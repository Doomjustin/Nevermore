#include <Nevermore/Timer.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <thread>
#include <iostream>


void test(int i)
{
    SPDLOG_INFO("i = {}", i);
}

void once_task()
{
    SPDLOG_INFO("this is an one time task");
}

struct A {
    void test_member(int x)
    {
        SPDLOG_INFO("this is a class task");
    }
};


int main()
{
    A a{};

    using namespace std::chrono_literals;
    SF::TimerManager timer_manager{};
    timer_manager.add_timer(5s, 2s, test, 1);
    timer_manager.add_timer(1s, once_task);
    timer_manager.add_timer(2s, 2s, &A::test_member, a, 1);

    timer_manager.run();

    return EXIT_SUCCESS;
}