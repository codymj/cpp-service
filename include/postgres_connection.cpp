#include "postgres_connection.hpp"

PqxxPtr PostgresConnection::build()
{
    return std::make_unique<pqxx::connection>(connectionString());
}

std::string PostgresConnection::connectionString()
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
