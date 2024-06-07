#pragma once

#include "user_model.hpp"
#include <connection_pool.hpp>
#include <postgres_connection.hpp>
#include <vector>
#include <spdlog/spdlog.h>

/**
 * Data store represented by a PostgreSQL database for storing User data.
 */
class PostgresUserStore
{
public:
    /**
     * Interface to save Users to the PostgreSQL database.
     * @param connectionPool is the PostgreSQL connection pool.
     */
    explicit PostgresUserStore(ConnectionPool<PqxxPtr>* connectionPool)
    : m_connectionPool(connectionPool)
    {
        m_logger = spdlog::get("logger");
    }

    /**
     * Gets all users from the database.
     * TODO: Implement query parameters.
     * @return All users in database.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>> getUsers() const;

    /**
     * Saves user to the database.
     * @param user is the User to save.
     */
    void saveUser(User const& user) const;

private:
    /**
     * Logger for this class.
     */
    std::shared_ptr<spdlog::logger> m_logger;

    /**
     * PostgreSQL connection pool.
     */
    ConnectionPool<PqxxPtr>* m_connectionPool;
};
