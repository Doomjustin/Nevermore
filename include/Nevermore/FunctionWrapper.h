#ifndef NEVERMORE_FUNCTION_WRAPPER_H
#define NEVERMORE_FUNCTION_WRAPPER_H

#include <memory>
#include <utility>


namespace SF {

// 类型擦除包装类
class FunctionWrapper {
public:
    FunctionWrapper()
      : callable_{ nullptr }
    {}

    // copy func 
    template<typename F>
    FunctionWrapper(F f)
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

private:
    struct Callable {
    public:
        ~Callable() {}

        virtual void call() noexcept = 0;
    };

    template<typename F>
    struct Functor: public Callable {
        F f;

        Functor(F&& func)
        : f{ std::forward<F>(func) }
        {}

        ~Functor() {}

        void call() noexcept override
        {
        f();
        }

    };

    std::unique_ptr<Callable> callable_;
};

} // namespace SF

#endif // NEVERMORE_FUNCTION_WRAPPER_H