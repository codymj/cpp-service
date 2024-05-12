#pragma once

#include "../store/user/user_store.hpp"
#include <memory>
#include <utility>

class StoreRegistry
{
public:
    StoreRegistry() = delete;

    StoreRegistry(StoreRegistry&) = delete;

    explicit StoreRegistry(std::shared_ptr<ConnectionPool> connectionPool)
    : m_connectionPool(std::move(connectionPool))
    {
        // Data stores get created here.
        m_userStore = std::make_shared<UserStore>(m_connectionPool);
    };

    /**
     * Returns a shared pointer to the UserStore.
     * @return Shared pointer to UserStore.
     */
    std::shared_ptr<UserStore>
    getUserStore();

private:
    /**
     * Connection pool shared pointer which gets copied to each data store.
     */
    std::shared_ptr<ConnectionPool> m_connectionPool;

    /**
     * Data store for User logic.
     */
    std::shared_ptr<UserStore> m_userStore;
};
