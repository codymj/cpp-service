#pragma once

#include <functional>

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define DEFER(code) Defer CONCATENATE(_defer_, __COUNTER__)([&](){code;})

/**
 * Defer is used to execute a function upon exiting a scope.
 * Example:
 * DEFER(m_connectionPool->freeConnection(std::move(cxn)));
 */
class Defer
{
public:
    /**
     * Constructor.
     * @param func Function to defer on scope exit.
     */
    explicit Defer(std::function<void()> func)
    : m_func(std::move(func)), m_active(true)
    {}

    /**
     * Disable copy constructor.
     */
    Defer(const Defer&) = delete;

    /**
     * Disable copy assignment.
     */
    Defer& operator=(const Defer&) = delete;

    /**
     * Enable move constructor.
     */
    Defer(Defer&& other) noexcept
    : m_func(std::move(other.m_func))
    , m_active(other.m_active)
    {
        other.m_active = false;
    }

    /**
     * Enable move assignment.
     * @param other Defer object to move.
     * @return Reference to moved Defer object.
     */
    Defer& operator=(Defer&& other) noexcept
    {
        if (this != &other)
        {
            m_func = std::move(other.m_func);
            m_active = other.m_active;
            other.m_active = false;
        }

        return *this;
    }

    /**
     * Destructor.
     */
    ~Defer()
    {
        if (m_active)
        {
            m_func();
        }
    }

private:
    std::function<void()> m_func;
    bool m_active;
};
