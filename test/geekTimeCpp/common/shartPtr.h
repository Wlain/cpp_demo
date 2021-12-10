//
// Created by william on 2021/12/9.
//

#ifndef CPP_DEMO_SHARTPTR_H
#define CPP_DEMO_SHARTPTR_H
#include <atomic>  // std::atomic
#include <utility> // std::swap
namespace geekTimeTest
{
class SharedCount
{
public:
    SharedCount() noexcept :
        m_count(1)
    {}

    void addCount() noexcept
    {
        m_count.fetch_add(1, std::memory_order_relaxed);
    }
    long reduceCount() noexcept
    {
        return --m_count;
    }
    long getCount() noexcept
    {
        return m_count;
    }

private:
    std::atomic_long m_count; // 原子即互斥
};

template <typename T>
class SharedPtr
{
public:
    /// 全部friend
    template <typename U>
    friend class SharedPtr;

    explicit SharedPtr(T* ptr = nullptr) :
        m_ptr(ptr)
    {
        if (m_ptr)
        {
            m_sharedCount = new SharedCount();
        }
    }

    ~SharedPtr()
    {
        if (m_ptr && !m_sharedCount->reduceCount())
        {
            delete m_ptr;
            delete m_sharedCount;
        }
    }
    SharedPtr(const SharedPtr& other) noexcept
    {
        m_ptr = other.m_ptr;
        if (m_ptr)
        {
            other.m_sharedCount->addCount();
            m_sharedCount = other.m_sharedCount;
        }
    }

    SharedPtr(const SharedPtr&& other) noexcept
    {
        m_ptr = other.m_ptr;
        if (m_ptr)
        {
            other.m_sharedCount->addCount();
            m_sharedCount = other.m_sharedCount;
        }
    }

    template <typename U>
    SharedPtr(const SharedPtr<U>& other) noexcept
    {
        m_ptr = other.m_ptr;
        if (m_ptr)
        {
            other.m_sharedCount->addCount();
            m_sharedCount = other.m_sharedCount;
        }
    }

    template <typename U>
    SharedPtr(SharedPtr<U>&& other) noexcept
    {
        m_ptr = other.m_ptr;
        if (m_ptr)
        {
            other.m_sharedCount->addCount();
            m_sharedCount = other.m_sharedCount;
        }
    }

    template <typename U>
    SharedPtr(const SharedPtr<U>& other, T* ptr) noexcept
    {
        m_ptr = ptr;
        if (m_ptr)
        {
            other.m_sharedCount->addCount();
            m_sharedCount = other.m_sharedCount;
        }
    }

    SharedPtr& operator=(SharedPtr rhs) noexcept
    {
        rhs.swap(*this);
        return *this;
    }

    T* get() const noexcept
    {
        return m_ptr;
    }

    long useCount() const noexcept
    {
        return m_ptr ? m_sharedCount->getCount() : 0;
    }

    void swap(SharedPtr& rhs) noexcept
    {
        std::swap(m_ptr, rhs.m_ptr);
        std::swap(m_sharedCount, rhs.m_sharedCount);
    }

    T& operator*() noexcept
    {
        return *m_ptr;
    }

    T* operator->() const noexcept
    {
        return m_ptr;
    }

    operator bool() const noexcept
    {
        return m_ptr;
    }

private:
    T* m_ptr;
    SharedCount* m_sharedCount;
};

template <typename T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, typename U>
SharedPtr<T> staticPointCast(const SharedPtr<U>& other) noexcept
{
    T* ptr = static_cast<T*>(other.get());
    return SharedPtr<T>(other, ptr);
}

template <typename T, typename U>
SharedPtr<T> reinterpretCast(const SharedPtr<U>& other) noexcept
{
    T* ptr = reinterpret_cast<T*>(other.get());
    return SharedPtr<T>(other, ptr);
}

template <typename T, typename U>
SharedPtr<T> constPointerCast(const SharedPtr<U>& other) noexcept
{
    T* ptr = const_cast<T*>(other.get());
    return SharedPtr<T>(other, ptr);
}

/// 基类转子类
template <typename T, typename U>
SharedPtr<T> dynamicPointerCast(const SharedPtr<U>& other) noexcept
{
    T* ptr = dynamic_cast<T*>(other.get());
    return SharedPtr<T>(other, ptr);
}
} // namespace geekTimeTest

#endif //CPP_DEMO_SHARTPTR_H