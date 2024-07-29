#pragma once

#include "postgres_connection.hpp"
#include "../store/user/postgres_user_store.hpp"
#include <memory>

/**
 * Holds all data stores which gets injected into the service registry.
 */
class store_registry
{
public:
    /**
     * Initializer for the store registry.
     * @param pg_connection_pool Pointer to a PostgreSQL connection pool.
     */
    explicit store_registry
    (
        connection_pool<pqxx_ptr>* pg_connection_pool
    )
    : m_pg_connection_pool(pg_connection_pool)
    {
        // Data stores get created here.
        m_user_store = std::make_unique<postgres_user_store>(m_pg_connection_pool);
    }

    /**
     * Returns a pointer to the UserStore.
     * @return Pointer to UserStore.
     */
    [[nodiscard]] postgres_user_store* get_user_store() const;

private:
    /**
     * PostgreSQL connection pool which gets injected to data stores.
     */
    connection_pool<pqxx_ptr>* m_pg_connection_pool{};

    /**
     * Data store for User logic.
     */
    std::unique_ptr<postgres_user_store> m_user_store;
};
