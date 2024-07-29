#pragma once

#include <functional>

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define DEFER(code) defer CONCATENATE(_defer_, __COUNTER__)([&](){code;})

/**
 * Used to execute a function upon exiting a scope.
 */
class defer
{
public:
    /**
     * Constructor.
     * @param func Function to defer on scope exit.
     */
    explicit defer(std::function<void()> func)
    : m_func(std::move(func)), m_active(true)
    {}

    /**
     * Disable copy constructor.
     */
    defer(const defer&) = delete;

    /**
     * Disable copy assignment.
     */
    defer& operator=(const defer&) = delete;

    /**
     * Enable move constructor.
     */
    defer(defer&& other) noexcept
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
    defer& operator=(defer&& other) noexcept
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
    ~defer()
    {
        if (m_active)
        {
            m_func();
        }
    }

private:
    /**
     * Function to run when scope is exited.
     */
    std::function<void()> m_func;

    /**
     * If true, runs m_func on scope exit.
     */
    bool m_active;
};
