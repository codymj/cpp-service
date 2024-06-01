#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * Connection pool for holding a container of connections to a data store.
 * @tparam T Type of connection for the pool to utilize.
 */
template <typename T>
class ConnectionPool
{
public:
    /**
     * Constructor for creating a connection pool.
     * @param connections Collection of connection objects.
     */
    explicit ConnectionPool(std::vector<T> connections)
    {
        std::lock_guard lock(m_mutex);

        // Populate the connection pool.
        for (auto&& cxn : connections)
        {
            m_pool.emplace(std::move(cxn));
        }
    }

    /**
     * Returns a database connection from the connection pool.
     * @return Database connection from connection pool.
     */
    [[nodiscard]] T rentConnection()
    {
        std::unique_lock lock(m_mutex);

        // If all connections are busy, wait until we can get a connection.
        while (m_pool.empty())
        {
            m_condition.wait(lock);
        }

        // Connection is ready, grab one.
        auto cxn = std::move(m_pool.front());
        m_pool.pop();

        return cxn;
    }

    /**
     * Frees the database connection back into the connection pool.
     */
    void freeConnection(T connection)
    {
        std::unique_lock lock(m_mutex);

        // Return connection to pool.
        m_pool.push(std::move(connection));

        lock.unlock();
        m_condition.notify_one();
    }

private:
    std::queue<T> m_pool;
    std::condition_variable m_condition;
    std::mutex m_mutex;
};
