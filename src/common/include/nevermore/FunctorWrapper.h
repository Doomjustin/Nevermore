#ifndef NEVERMORE_COMMON_FUNCTOR_WRAPPER_H
#define NEVERMORE_COMMON_FUNCTOR_WRAPPER_H

#include <memory>

namespace sf {

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

} // namespace sf

#endif // NEVERMORE_COMMON_FUNCTOR_WRAPPER_H