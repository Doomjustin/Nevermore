#ifndef NEVERMORE_UTILITY_THREAD_POOL_H
#define NEVERMORE_UTILITY_THREAD_POOL_H

#include <thread>
#include <future>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <type_traits>

namespace sf {

namespace details {
    
class FunctionWrapper {
public:
    FunctionWrapper()
      : callable_{ nullptr }
    {}

    // copy func
    template<typename F>
    explicit FunctionWrapper(F f)
      : callable_{ std::make_unique<Functor<F>>(std::move(f)) }
    {}

    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;

    FunctionWrapper(FunctionWrapper&&) noexcept = default;
    FunctionWrapper& operator=(FunctionWrapper&&) noexcept = default;

    ~FunctionWrapper() = default;

    void operator()() noexcept
    {
        callable_->call();
    }

    explicit operator bool() const noexcept { return static_cast<bool>(callable_); }

private:
    struct Callable {
    public:
        virtual ~Callable() = default;

        virtual void call() noexcept = 0;
    };

    template<typename F>
    struct Functor final: public Callable {
        F f;

        explicit Functor(F&& func)
          : f{ std::forward<F>(func) }
        {}

        void call() noexcept override
        {
            f();
        }
    };

    std::unique_ptr<Callable> callable_;
};

} // namespace sf::details


class ThreadPool {
public:
    ThreadPool();
    explicit ThreadPool(int max_threads);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&& other) noexcept;
    ThreadPool& operator=(ThreadPool&& other) noexcept;

    ~ThreadPool();

    template<typename F, typename... Args>
        requires std::invocable<F, Args...>
    std::future<std::invoke_result_t<F, Args...>> submit(F f, Args&&... args)
    {
        using result_type = std::invoke_result_t<F, Args...>;

        std::packaged_task<result_type()> task{ std::bind(std::move(f), std::forward<Args>(args)...) };
        auto res = task.get_future();

        {
            std::unique_lock<std::mutex> lock{ m_ };
            tasks_.emplace(std::move(task));
            cv_.notify_one();
        }

        return res;
    }

    constexpr bool stopped() const noexcept { return !done_; }
    bool completed() const noexcept { return tasks_.empty(); }

    void stop();

    void restart();

private:
    static constexpr int max_threads_ = 16;

    bool done_;
    std::mutex m_;
    std::condition_variable cv_;
    std::vector<std::thread> threads_;
    std::queue<details::FunctionWrapper> tasks_;

    void thread_work();
};

} // namespace sf

#endif // NEVERMORE_UTILITY_THREAD_POOL_H