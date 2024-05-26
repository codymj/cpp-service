#pragma once

#include "postgres_connection.hpp"
#include "../store/user/postgres_user_store.hpp"
#include <memory>
#include <utility>

/**
 * Registry to hold all data stores which gets injected into the
 * ServiceRegistry.
 */
class StoreRegistry
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    StoreRegistry() = delete;
    StoreRegistry(StoreRegistry&) = delete;
    StoreRegistry(StoreRegistry&&) = delete;

    explicit StoreRegistry
    (
        ConnectionPool<PqxxPtr>* connectionPool
    )
    : m_connectionPool(connectionPool)
    {
        // Data stores get created here.
        m_userStore = std::make_unique<PostgresUserStore>(m_connectionPool);
    };

    /**
     * Returns a pointer to the UserStore.
     * @return Pointer to UserStore.
     */
    PostgresUserStore*
    getUserStore();

private:
    /**
     * Connection pool shared pointer which gets copied to each data store.
     */
    ConnectionPool<PqxxPtr>* m_connectionPool{};

    /**
     * Data store for User logic.
     */
    std::unique_ptr<PostgresUserStore> m_userStore;
};
