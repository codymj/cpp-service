#include "store_registry.hpp"
#include <quill/LogMacros.h>

void store_registry::create_postgres_connection_pool
(
    config_manager const* config_manager
)
{
    try
    {
        std::string const& host = config_manager->database_host();
        uint16_t const port = config_manager->database_port();
        std::string const& username = config_manager->database_username();
        std::string const& password = config_manager->database_password();
        std::string const& name = config_manager->database_name();
        uint16_t const connection_timeout =
            config_manager->database_connection_timeout();
        uint16_t const pool_size =
            config_manager->database_connection_pool_size();

        // Build database connections.
        std::vector<pqxx_ptr> connections;
        connections.reserve(pool_size);

        auto const cxn = postgres_connection
        (
            host, port, username, password, name, connection_timeout
        );
        for (auto i=0; i<pool_size; ++i)
        {
            connections.emplace_back(cxn.build());
        }

        m_pg_connection_pool = std::make_unique<connection_pool<pqxx_ptr>>
        (
            std::move(connections)
        );
    }
    catch (std::exception const& e)
    {
        LOG_CRITICAL
        (
            quill::Frontend::get_logger("root"),
            "Error in create_postgres_connection_pool: {error}",
            e.what()
        );
        std::exit(EXIT_FAILURE);
    }
}

postgres_user_store* store_registry::get_user_store() const
{
    return m_user_store.get();
}
