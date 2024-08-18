#pragma once

#include "user_model.hpp"
#include <connection_pool.hpp>
#include <postgres_connection.hpp>
#include <vector>
#include <quill/Frontend.h>

/**
 * Data store represented by a PostgreSQL database for storing User data.
 */
class postgres_user_store
{
public:
    /**
     * Interface to save Users to the PostgreSQL database.
     * @param connection_pool is the PostgreSQL connection pool.
     */
    explicit postgres_user_store(connection_pool<pqxx_ptr>* connection_pool)
    : m_connection_pool(connection_pool)
    {}

    /**
     * Gets all users from the database.
     * TODO: Implement query parameters.
     * @return All users in database.
     */
    [[nodiscard]] std::unique_ptr<std::vector<user>> get_users() const;

    /**
     * Saves user to the database.
     * @param user is the user to save.
     */
    void save_user(user const& user) const;

private:
    /**
     * Logger.
     */
    quill::Logger* m_logger = quill::Frontend::get_logger("root");

    /**
     * PostgreSQL connection pool.
     */
    connection_pool<pqxx_ptr>* m_connection_pool;
};
