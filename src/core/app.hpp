#pragma once

#include "router.hpp"
#include "service_registry.hpp"
#include "store_registry.hpp"
#include <config_manager.hpp>
#include <filesystem>
#include <postgres_connection.hpp>
#include <memory>

class app
{
public:
    /**
     * Initializes application.
     */
    void initialize(std::filesystem::path const&);

    /**
     * Main application loop for the HTTP server.
     * @return Exit code.
     */
    int main();

private:
    /**
     * Loads application configuration properties.
     */
    void load_configuration(std::filesystem::path const&);

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
    std::unique_ptr<connection_pool<pqxx_ptr>> m_pg_connection_pool;
    std::unique_ptr<store_registry> m_store_registry;
    std::unique_ptr<service_registry> m_service_registry;
    std::unique_ptr<router> m_router;
};
