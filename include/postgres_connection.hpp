#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>

using pqxx_ptr = std::unique_ptr<pqxx::connection>;

/**
 * Class for building a PostgreSQL connection.
 */
class postgres_connection
{
public:
    /**
     * Constructor for building a connection to a PostgreSQL database.
     * @param host Database host.
     * @param port Database port.
     * @param username Username to connect to database.
     * @param password Password to connect to database.
     * @param name Database name.
     * @param connection_timeout Connection timeout in seconds.
     */
    postgres_connection
    (
        std::string host,
        uint16_t const port,
        std::string username,
        std::string password,
        std::string name,
        uint16_t const connection_timeout = 180
    )
    : m_host(std::move(host))
    , m_username(std::move(username))
    , m_password(std::move(password))
    , m_name(std::move(name))
    , m_port(port)
    , m_connection_timeout(connection_timeout)
    {}

    /**
     * Builds and returns the pointer to the database connector.
     * @return
     */
    [[nodiscard]] pqxx_ptr build() const
    {
        return std::make_unique<pqxx::connection>(connection_string());
    }

private:
    /**
     * Builds the connection string based on the provided parameters.
     * @return PostgreSQL connection string.
     */
    [[nodiscard]] std::string connection_string() const
    {
        return std::string
            {
                "postgresql://" +
                m_username + ":" + m_password + "@" +
                m_host + ":" +
                std::to_string(m_port) + "/" +
                m_name + "?" +
                "connect_timeout=" + std::to_string(m_connection_timeout)
            };
    }

    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_name;
    uint16_t m_port;
    uint16_t m_connection_timeout;
};
