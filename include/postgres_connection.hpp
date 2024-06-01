#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>
#include "connection_pool.hpp"

using PqxxPtr = std::unique_ptr<pqxx::connection>;

/**
 * Class for building a PostgreSQL connection.
 */
class PostgresConnection
{
public:
    /**
     * Constructor for building a connection to a PostgreSQL database.
     * @param host Database host.
     * @param port Database port.
     * @param username Username to connect to database.
     * @param password Password to connect to database.
     * @param name Database name.
     * @param connectionTimeout Connection timeout in seconds.
     */
    PostgresConnection
    (
        std::string host,
        uint16_t const port,
        std::string username,
        std::string password,
        std::string name,
        uint16_t const connectionTimeout = 180
    )
    : m_host(std::move(host))
    , m_username(std::move(username))
    , m_password(std::move(password))
    , m_name(std::move(name))
    , m_port(port)
    , m_connectionTimeout(connectionTimeout)
    {}

    /**
     * Builds and returns the pointer to the database connector.
     * @return
     */
    [[nodiscard]] PqxxPtr build() const
    {
        return std::make_unique<pqxx::connection>(connectionString());
    }

private:
    /**
     * Builds the connection string based on the provided parameters.
     * @return PostgreSQL connection string.
     */
    [[nodiscard]] std::string connectionString() const
    {
        return std::string
            {
                "postgresql://" +
                m_username + ":" + m_password + "@" +
                m_host + ":" +
                std::to_string(m_port) + "/" +
                m_name + "?" +
                "connect_timeout=" + std::to_string(m_connectionTimeout)
            };
    }

    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_name;
    uint16_t m_port;
    uint16_t m_connectionTimeout;
};
