#include "postgres_connection.hpp"

PostgresConnectionPtr PostgresConnection::build()
{
    return std::make_unique<pqxx::connection>(connection_string());
}

std::string PostgresConnection::connection_string()
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
