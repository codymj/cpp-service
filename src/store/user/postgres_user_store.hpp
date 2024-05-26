#pragma once

#include "connection_pool.hpp"
#include "postgres_connection.hpp"
#include "user_model.hpp"
#include <vector>

using cppservice::database::ConnectionPool;

class PostgresUserStore
{
public:
    explicit PostgresUserStore(ConnectionPool<PqxxPtr>* connectionPool)
    : m_connectionPool(connectionPool)
    {}

    /**
     * Gets all users from the database.
     * TODO: Implement query parameters.
     * @return All users in database.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

private:
    ConnectionPool<PqxxPtr>* m_connectionPool;
};
