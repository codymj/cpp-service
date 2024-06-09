#pragma once

#include <memory>
#include <mutex>
#include <Poco/UUIDGenerator.h>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

class Context final
{
public:
    using Value = std::variant<int, double, std::string>;

    /**
     * Construct a context and set any default values in map.
     * @op
     */
    explicit Context()
    : m_startTime(std::chrono::steady_clock::now())
    {
        auto uuidGenerator = Poco::UUIDGenerator();
        set("traceId", uuidGenerator.createRandom().toString());
    }

    std::chrono::time_point<std::chrono::steady_clock> startTime() const
    {
        return m_startTime;
    }

    /**
     * Sets a key/value pair in the context map.
     * @param key Key to store.
     * @param value Value to store.
     */
    void set(std::string const& key, Value const& value)
    {
        std::lock_guard lock(m_mutex);
        m_values[key] = value;
    }

    /**
     * Get value from context map.
     * @param key Key of the value to return.
     * @return Value at key.
     */
    template <typename T>
    [[nodiscard]] T get(std::string const& key) const
    {
        std::lock_guard lock(m_mutex);
        auto const it = m_values.find(key);
        if (it != m_values.end())
        {
            return std::get<T>(it->second);
        }

        throw std::runtime_error(std::string(key + " not found in context"));
    }

    /**
     * Returns map of key/values.
     * @return Map of key/values.
     */
    [[nodiscard]] std::unordered_map<std::string, Value>& getValues()
    {
        return m_values;
    }

    /**
     *
     */
    [[nodiscard]] std::string traceId() const
    {
        return std::string("[" + get<std::string>("traceId") + "]");
    }

    /**
     * Gets context from thread local storage.
     * @return Context in thread local storage.
     */
    static std::shared_ptr<Context> getContext()
    {
        return m_context;
    }

    /**
     * Sets context to thread local storage.
     * @param context Context to set in thread local storage.
     */
    static void setContext(std::shared_ptr<Context> const& context)
    {
        m_context = context;
    }

private:
    /**
     * Unordered map to contain context key/value pairs.
     */
    std::unordered_map<std::string, Value> m_values;

    /**
     * Access guard to the map.
     */
    mutable std::mutex m_mutex;

    /**
     * Time when context was created.
     */
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;

    /**
     * Thread local storage to maintain the life of the context.
     */
    static inline thread_local std::shared_ptr<Context> m_context;
};
