#pragma once

#include "postgres_connection.hpp"
#include "../store/user/postgres_user_store.hpp"
#include <config_manager.hpp>
#include <memory>

/**
 * Holds all data stores which gets injected into the service registry. The
 * store registry is instantiated on application start up and owns the data
 * stores which get forwarded to the services.
 */
class store_registry
{
public:
    /**
     * Initializer for the store registry.
     * @param config_manager Pointer to the configuration manager which is
     * managed in app.
     */
    explicit store_registry(config_manager const* config_manager)
    {
        // Initialize database.
        create_postgres_connection_pool(config_manager);

        // Data stores get created here.
        m_user_store = std::make_unique<postgres_user_store>
        (
            m_pg_connection_pool.get()
        );
    }

    /**
     * Returns a pointer to the UserStore.
     * @return Pointer to UserStore.
     */
    [[nodiscard]] postgres_user_store* get_user_store() const;

private:
    /**
     * Creates the connection pool for the database with parameters from
     * the app.json file. The pool gets injected into the StoreRegistry.
     * @return Shared pointer of the connection pool.
     */
    void create_postgres_connection_pool(config_manager const*);

    /**
     * PostgreSQL connection pool which gets injected to data stores.
     */
    std::unique_ptr<connection_pool<pqxx_ptr>> m_pg_connection_pool;

    /**
     * Data store for User logic.
     */
    std::unique_ptr<postgres_user_store> m_user_store;
};
