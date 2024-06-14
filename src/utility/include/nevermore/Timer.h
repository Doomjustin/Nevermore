#ifndef NEVERMORE_UTILITY_TIMER_H
#define NEVERMORE_UTILITY_TIMER_H

#include <functional>
#include <chrono>

#include "Heap.h"
#include "ThreadPool.h"
#include "Singleton.h"

namespace sf {

namespace details {

template<typename Clock, typename Duration>
class BasicTimer {
public:
    using ClockType = Clock;
    using DurationType = Duration;
    using TimePointType = std::chrono::time_point<ClockType, DurationType>;
    using CallableType = std::function<void()>;

public:
    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    BasicTimer(DurationType run_after, DurationType repeat_interval, F func, Args... args)
      : expiry_{ ClockType::now() + run_after },
        interval_{ repeat_interval }
    {
        task_ = std::bind(std::move(func), std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    BasicTimer(DurationType run_after, F func, Args... args)
      : expiry_{ ClockType::now() + run_after },
        interval_{}
    {
        task_ = std::bind(std::move(func), std::forward<Args>(args)...);
    }

    constexpr TimePointType expiry() const noexcept { return expiry_; }

    constexpr DurationType interval() const noexcept { return interval_; }

    void interval(DurationType new_interval) noexcept { interval_ = new_interval; }

    constexpr bool should_repeat() const noexcept { return interval_ != DurationType::zero(); }

    void update() { expiry_ += interval_; }

    void run()
    {
        task_();
    }

private:
    TimePointType expiry_;
    DurationType interval_;
    CallableType task_;
};

template<typename Clock, typename Duration>
bool operator>(const BasicTimer<Clock, Duration>& lhs, const BasicTimer<Clock, Duration>& rhs)
{
    return lhs.expiry() > rhs.expiry();
}

template<typename Clock, typename Duration>
bool operator<(const BasicTimer<Clock, Duration>& lhs, const BasicTimer<Clock, Duration>& rhs)
{
    return lhs.expiry() < rhs.expiry();
}

} // namespace sf::details


template<typename Clock, typename Duration>
class BasicTimerManager {
public:
    using ClockType = Clock;
    using DurationType = Duration;
    using TimePointType = std::chrono::time_point<ClockType, DurationType>;
    using TimerType = details::BasicTimer<Clock, Duration>;

public:
    BasicTimerManager()
      : thread_pool_{ make_singleton<ThreadPool>() }
    {}

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    void add_timer(DurationType run_after, DurationType repeat_interval, F func, Args... args)
    {
        timers_.emplace(run_after, repeat_interval, std::move(func), std::forward<Args>(args)...);
    }

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    void add_timer(DurationType run_after, F func, Args... args)
    {
        timers_.emplace(run_after, std::move(func), std::forward<Args>(args)...);
    }

    void add_timer(const TimerType& timer)
    {
        timers_.push(timer);
    }

    void ticks()
    {
        if (!has_timer()) return;

        auto cur_time = ClockType::now();

        TimerType timer = timers_.get();

        while (timer.expiry() <= cur_time) {
            thread_pool_.submit(&TimerType::run, timer);
            if (timer.should_repeat()) 
            {
                timer.update();
                timers_.push(timer);
            }

            timers_.pop();
            timer = timers_.get();
        }
    }

    void run()
    {
        // FIXME: make it stoppable
        while (true)
            ticks();
    }

    constexpr bool has_timer() const noexcept { return !timers_.empty(); }

private:
    ThreadPool& thread_pool_;
    MinHeap<TimerType> timers_;
};

using TimerManager = BasicTimerManager<std::chrono::steady_clock, std::chrono::nanoseconds>;

} // namespace sf

#endif // NEVERMORE_UTILITY_TIMER_H