#ifndef NEVERMORE_UTILITY_DELEGATE_H
#define NEVERMORE_UTILITY_DELEGATE_H

#include <functional>
#include <memory>

namespace sf {

namespace details {

template<typename Ret, typename... Args>
struct Base {
    virtual ~Base() = default;

    virtual Ret func(Args&&... args) = 0;
};

template<typename T, typename Ret, typename... Args>
class MemberFunctionWrapper: public Base<Ret, Args...> {
public:
    MemberFunctionWrapper(T* ptr, Ret (T::*func)(Args...))
      : instance_ptr_{ ptr }, func_{ func }
    {}

    ~MemberFunctionWrapper() = default;

    Ret func(Args&&... args) override
    {
        return std::invoke(func_, instance_ptr_, std::forward<Args>(args)...);
    }

private:
    T* instance_ptr_;
    Ret (T::*func_)(Args...);
};

template<typename T, typename Ret, typename... Args>
class ConstMemberFunctionWrapper: public Base<Ret, Args...> {
public:
    ConstMemberFunctionWrapper(const T* ptr, Ret (T::*func)(Args...) const)
      : instance_ptr_{ ptr }, func_{ func }
    {}

    ~ConstMemberFunctionWrapper() = default;

    Ret func(Args&&... args) override
    {
        return std::invoke(func_, instance_ptr_, std::forward<Args>(args)...);
    }

private:
    const T* instance_ptr_;
    Ret (T::*func_)(Args...) const;
};

template<typename Ret, typename... Args>
class FunctionWrapper: public Base<Ret, Args...> {
public:
    explicit FunctionWrapper(Ret (*func)(Args...))
      : func_{ func }
    {}

    ~FunctionWrapper() = default;

    Ret func(Args&&... args) override
    {
        return func_(std::forward<Args>(args)...);
    }

private:
    Ret (*func_)(Args...);
};

} // namespace sf::details


template<typename Ret, typename... Args>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)> {
public:
    template<typename T>
    Delegate(T* t, Ret (T::*func)(Args...))
      : wrapper_{ std::make_unique<details::MemberFunctionWrapper<T, Ret, Args...>>(t, func) }
    {}

    template<typename T>
    Delegate(const T* t, Ret (T::*func)(Args...) const)
      : wrapper_{ std::make_unique<details::ConstMemberFunctionWrapper<T, Ret, Args...>>(t, func) }
    {}

    explicit Delegate(Ret (*func)(Args...))
      : wrapper_{ std::make_unique<details::FunctionWrapper<Ret, Args...>>(func) }
    {}

    Delegate(const Delegate&) = delete;
    Delegate& operator=(const Delegate) = delete;

    Delegate(Delegate&&) noexcept = default;
    Delegate& operator=(Delegate&&) noexcept = default;

    ~Delegate() = default;

    Ret operator()(Args&&... args)
    {
        return wrapper_->func(std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<details::Base<Ret, Args...>> wrapper_;
};

template<typename T, typename Ret, typename... Args>
Delegate(T*, Ret (T::*)(Args...)) -> Delegate<Ret(Args...)>;

template<typename T, typename Ret, typename... Args>
Delegate(const T*, Ret (T::*)(Args...) const) -> Delegate<Ret(Args...)>;

template<typename Ret, typename... Args>
explicit Delegate(Ret (*)(Args...)) -> Delegate<Ret(Args...)>;

} // namespace sf

#endif // NEVERMORE_UTILITY_DELEGATE_H
