#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>
#include "connection_pool.hpp"

using PostgresConnectionPtr = std::unique_ptr<pqxx::connection>;

class PostgresConnection
{
public:
    PostgresConnection
    (
        std::string host,
        uint16_t port,
        std::string username,
        std::string password,
        std::string name,
        uint8_t connectionTimeout = 180
    )
    : m_host(std::move(host))
    , m_username(std::move(username))
    , m_password(std::move(password))
    , m_name(std::move(name))
    , m_port(port)
    , m_connectionTimeout(connectionTimeout)
    {}

    PostgresConnectionPtr
    build();

private:
    std::string
    connection_string();

    std::string m_host;
    std::string m_username;
    std::string m_password;
    std::string m_name;
    uint16_t m_port;
    uint8_t m_connectionTimeout;
};
