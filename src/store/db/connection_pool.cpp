#include "connection_pool.hpp"

namespace cppservice::database {

void ConnectionPool::create() {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Build connection string to database.
    std::string cs{
        "postgresql://" +
        m_username + ":" + m_password + "@" +
        m_host + ":" +
        std::to_string(m_port) + "/" +
        m_name + "?" +
        "connect_timeout=" + std::to_string(m_connectionTimeout)
    };

    // Populate the connection pool.
    for (auto i=0; i<m_connectionPoolSize; ++i) {
        m_pool.emplace(
            std::make_shared<pqxx::connection>(pqxx::connection{cs})
        );
    }
}

std::shared_ptr<pqxx::connection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(m_mutex);

    // If all connections are busy, wait until we can get a connection.
    while (m_pool.empty()) {
        m_condition.wait(lock);
    }

    // Connection is ready, grab one.
    auto cxn = m_pool.front();
    m_pool.pop();

    return cxn;
}

void ConnectionPool::freeConnection(
std::shared_ptr<pqxx::connection> const& connection
) {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_pool.push(connection);

    lock.unlock();
    m_condition.notify_one();
}

}
