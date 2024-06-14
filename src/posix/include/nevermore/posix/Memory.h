#ifndef NEVERMORE_UTILITY_MEMORY_H
#define NEVERMORE_UTILITY_MEMORY_H

#include <cstddef>
#include <memory>
#include <new>
#include <utility>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

namespace sf {

namespace details {

constexpr auto ProtRW = PROT_READ | PROT_WRITE;
constexpr auto MapAlloc = MAP_PRIVATE | MAP_ANONYMOUS;
constexpr auto SharedMemoryName = "/shm";
constexpr auto SharedMemoryOpenMode = 644;

} // namespace sf::details 

class MmapDeleter {
public:
    explicit MmapDeleter(std::size_t size)
      : size_{ size }
    {}

    void operator()(int* ptr) const 
    {
        ::munmap(ptr, size_);
    }

private:
    std::size_t size_;
};

template<typename T, typename... Args>
auto mmap_unique(Args&&... args)
{
    if (auto* ptr = ::mmap(nullptr, 
                                   sizeof(T), 
                                   details::ProtRW, 
                                   details::MapAlloc, 
                                   -1, 
                                   0)) 
    {
        auto* obj = new (ptr) T(std::forward<Args>(args)...);
        MmapDeleter deleter{ sizeof(T) };

        return std::unique_ptr<T, MmapDeleter>(obj, deleter);
    }

    throw std::bad_alloc{};
}

// WARNING:不适用于复杂类型，比如有继承关系的类
template<typename T, typename... Args>
auto mmap_unique_server(Args&&... args)
{
    if (auto fd = ::shm_open(details::SharedMemoryName, 
                                  O_CREAT | O_RDWR, 
                                  details::SharedMemoryOpenMode); 
        fd != -1) 
    {
        ftruncate(fd, sizeof(T));

        if (auto* ptr = ::mmap(nullptr, 
                                       sizeof(T), 
                                       details::ProtRW, 
                                       MAP_SHARED, 
                                       fd, 
                                       0)) 
        {
            auto* obj = new (ptr) T(std::forward<Args>(args)...);
            MmapDeleter deleter{ sizeof(T) };

            return std::unique_ptr<T, MmapDeleter>(obj, deleter);
        }
    }

    throw std::bad_alloc{};
}


template<typename T>
auto mmap_unique_client()
{
    if (auto fd = ::shm_open(details::SharedMemoryName, 
                                  O_RDWR, 
                                  details::SharedMemoryOpenMode); 
        fd != -1) 
    {
        ftruncate(fd, sizeof(T));

        if (auto* ptr = ::mmap(nullptr, 
                                       sizeof(T), 
                                       details::ProtRW, 
                                       MAP_SHARED, 
                                       fd, 
                                       0)) 
        {
            auto* obj = static_cast<T*>(ptr);
            MmapDeleter deleter{ sizeof(T) };

            return std::unique_ptr<T, MmapDeleter>(obj, deleter);
        }
    }

    throw std::bad_alloc{};
}

} // namespace sf

#endif // NEVERMORE_UTILITY_MEMORY_H