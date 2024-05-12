#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <pqxx/pqxx>

namespace cppservice::database
{

// TODO: Separate connection and pool logic.
class ConnectionPool
{
public:
    ConnectionPool() = delete;

    ConnectionPool(ConnectionPool&) = delete;

    ConnectionPool
    (
        std::string host,
        uint16_t port,
        std::string username,
        std::string password,
        std::string name,
        uint8_t connectionTimeout = 180,
        uint8_t connectionPoolSize = 8
    )
    : m_host(std::move(host))
    , m_username(std::move(username))
    , m_password(std::move(password))
    , m_name(std::move(name))
    , m_port(port)
    , m_connectionTimeout(connectionTimeout)
    , m_connectionPoolSize(connectionPoolSize)
    {
        create();
    }

    /**
     * Returns a database connection from the connection pool.
     * @return Database connection from connection pool.
     */
    [[nodiscard]] std::shared_ptr<pqxx::connection>
    getConnection();

    /**
     * Frees the database connection back into the connection pool.
     */
    void
    freeConnection(std::shared_ptr<pqxx::connection> const&);

private:
    /**
     * Creates the connection pool with as many connections as
     * m_connectionPoolSize.
     */
    void
    create();

    std::queue<std::shared_ptr<pqxx::connection>> m_pool;
    std::condition_variable m_condition;
    std::mutex m_mutex;

    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_name;
    uint16_t m_port;
    uint8_t m_connectionTimeout;
    uint8_t m_connectionPoolSize;
};

} // namespace cppservice::database