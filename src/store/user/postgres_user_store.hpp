#pragma once

#include "user_model.hpp"
#include <connection_pool.hpp>
#include <postgres_connection.hpp>
#include <vector>
#include <pqxx/except>

/**
 * Data store represented by a PostgreSQL database for storing User data.
 */
class PostgresUserStore
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    PostgresUserStore() = delete;
    PostgresUserStore(PostgresUserStore&) = delete;
    PostgresUserStore(PostgresUserStore&&) = delete;

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

    void
    saveUser(User const& user);

private:
    ConnectionPool<PqxxPtr>* m_connectionPool;
};
