#pragma once

#include "postgres_connection.hpp"
#include "../store/user/postgres_user_store.hpp"
#include <memory>

/**
 * Registry to hold all data stores which gets injected into the
 * ServiceRegistry.
 */
class StoreRegistry
{
public:
    /**
     * Initializer for the store registry.
     * @param pgConnectionPool Pointer to a PostgreSQL connection pool.
     */
    explicit StoreRegistry
    (
        ConnectionPool<PqxxPtr>* pgConnectionPool
    )
    : m_pgConnectionPool(pgConnectionPool)
    {
        // Data stores get created here.
        m_userStore = std::make_unique<PostgresUserStore>(m_pgConnectionPool);
    };

    /**
     * Returns a pointer to the UserStore.
     * @return Pointer to UserStore.
     */
    [[nodiscard]] PostgresUserStore* getUserStore() const;

private:
    /**
     * PostgreSQL connection pool which gets injected to data stores.
     */
    ConnectionPool<PqxxPtr>* m_pgConnectionPool{};

    /**
     * Data store for User logic.
     */
    std::unique_ptr<PostgresUserStore> m_userStore;
};
