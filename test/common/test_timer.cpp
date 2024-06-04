#include <cstdlib>
#include <iostream>

#include "nevermore/Timer.h"

void test(int i)
{
    std::cout << "i = " << i << '\n';
}

void once_task()
{
    std::cout << "this is an one time task\n";
}

struct A {
    void test_member(int x)
    {
        std::cout << "this is a class task, " << x << '\n';
    }
};


int main()
{
    A a{};
    sf::TimerManager timer_manager{};

    using namespace std::chrono_literals;
    timer_manager.add_timer(5s, 2s, test, 1);
    timer_manager.add_timer(1s, once_task);
    timer_manager.add_timer(2s, 2s, &A::test_member, a, 1);

    timer_manager.run();

    return EXIT_SUCCESS;
}