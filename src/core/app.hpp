#pragma once

#include "router.hpp"
#include "service_registry.hpp"
#include "store_registry.hpp"
#include <config_manager.hpp>
#include <postgres_connection.hpp>
#include <memory>

class app
{
public:
    /**
     * Initializes application.
     */
    void initialize();

    /**
     * Main application loop for the HTTP server.
     * @return Exit code.
     */
    int main();

private:
    /**
     * Loads application configuration properties.
     */
    void load_configuration();

    /**
     * Initializes logger.
     */
    void init_logger() const;

    /**
     * Creates the connection pool for the database with parameters from
     * the app.json file. The pool gets injected into the StoreRegistry.
     * @return Shared pointer of the connection pool.
     */
    void create_postgres_connection_pool();

    std::unique_ptr<config_manager> m_config_manager;
    std::unique_ptr<ConnectionPool<PqxxPtr>> m_connectionPool;
    std::unique_ptr<StoreRegistry> m_storeRegistry;
    std::unique_ptr<ServiceRegistry> m_serviceRegistry;
    std::unique_ptr<router> m_router;
};
