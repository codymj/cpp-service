#pragma once

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <variant>

/**
 * Context object to manage request parameters through the application.
 * TODO: Implement timeouts and other things.
 */
class context final
{
public:
    /**
     * Possible types a value can take in the map.
     */
    using value = std::variant<int, double, std::string>;

    /**
     * Construct a context and set any default values that all contexts should
     * have.
     */
    explicit context()
    : m_start_time(std::chrono::steady_clock::now())
    {
        set("trace_id", to_string(boost::uuids::random_generator()()));
    }

    /**
     * Returns the start time of the request.
     * @return When the request began.
     */
    std::chrono::time_point<std::chrono::steady_clock> start_time() const
    {
        return m_start_time;
    }

    /**
     * Sets a key/value pair in the context map.
     * @param key Key to store.
     * @param value Value to store.
     */
    void set(std::string const& key, value const& value)
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
    [[nodiscard]] std::unordered_map<std::string, value>& values()
    {
        return m_values;
    }

    /**
     * Returns traceId for logging.
     */
    [[nodiscard]] std::string trace_id() const
    {
        return std::string(get<std::string>("trace_id"));
    }

    /**
     * Gets context from thread local storage.
     * @return Context in thread local storage.
     */
    static std::shared_ptr<context> get_context()
    {
        return m_context;
    }

    /**
     * Sets context to thread local storage.
     * @param context Context to set in thread local storage.
     */
    static void set_context(std::shared_ptr<context> const& context)
    {
        m_context = context;
    }

private:
    /**
     * Unordered map to contain context key/value pairs.
     */
    std::unordered_map<std::string, value> m_values;

    /**
     * Access guard to the map.
     */
    mutable std::mutex m_mutex;

    /**
     * Time when context was created.
     */
    std::chrono::time_point<std::chrono::steady_clock> m_start_time;

    /**
     * Thread local storage to maintain the life of the context.
     */
    static inline thread_local std::shared_ptr<context> m_context;
};
