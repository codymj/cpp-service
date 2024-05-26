#pragma once

#include "postgres_connection.hpp"
#include "../store/user/user_store.hpp"
#include <memory>
#include <utility>

/**
 * Registry to hold all data stores which gets injected into the
 * ServiceRegistry.
 */
class StoreRegistry
{
public:
    StoreRegistry() = delete;

    StoreRegistry(StoreRegistry&) = delete;

    StoreRegistry(StoreRegistry&&) = delete;

    explicit StoreRegistry
    (
        ConnectionPool<PostgresConnectionPtr>* connectionPool
    )
    : m_connectionPool(connectionPool)
    {
        // Data stores get created here.
        m_userStore = std::make_unique<UserStore>(m_connectionPool);
    };

    /**
     * Returns a pointer to the UserStore.
     * @return Pointer to UserStore.
     */
    UserStore*
    getUserStore();

private:
    /**
     * Connection pool shared pointer which gets copied to each data store.
     */
    ConnectionPool<PostgresConnectionPtr>* m_connectionPool;

    /**
     * Data store for User logic.
     */
    std::unique_ptr<UserStore> m_userStore;
};
