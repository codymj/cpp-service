#include "app.hpp"
#include <boost/beast/core.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <config_manager.hpp>
#include <exception>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "listener.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

void app::initialize(std::filesystem::path const& path)
{
    load_configuration(path);
    init_logger();
    create_postgres_connection_pool();
}

void app::load_configuration(std::filesystem::path const& path)
{
    try
    {
        m_config_manager = std::make_unique<config_manager>(path);
    }
    catch (YAML::ParserException const& e)
    {
        SPDLOG_CRITICAL
        (
            "Error parsing configuration properties: {}",
            e.what()
        );
        std::exit(EXIT_FAILURE);
    }
    catch (YAML::BadConversion const& e)
    {
        SPDLOG_CRITICAL
        (
            "Error converting property from properties.yml: {}",
            e.what()
        );
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception const& e)
    {
        SPDLOG_CRITICAL
        (
            "Unknown error while loading configuration properties: {}",
            e.what()
        );
        std::exit(EXIT_FAILURE);
    }
}

void app::init_logger() const
{
    try
    {
        std::string const level = m_config_manager->app_log_level();

        if (level == "trace")
            spdlog::set_level(spdlog::level::trace);
        else if (level == "warn")
            spdlog::set_level(spdlog::level::warn);
        else if (level == "error")
            spdlog::set_level(spdlog::level::err);
        else if (level == "critical")
            spdlog::set_level(spdlog::level::critical);
        else
            spdlog::set_level(spdlog::level::debug);
    }
    catch (std::exception& e)
    {
        SPDLOG_CRITICAL("Error initializing logger: {}", e.what());
        std::exit(EXIT_FAILURE);
    }

    spdlog::init_thread_pool(8192, 1);
    auto const console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto const m_logger = std::make_shared<spdlog::async_logger>
    (
        "logger",
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::block
    );
    register_logger(m_logger);
    set_default_logger(m_logger);
    m_logger->set_pattern("[%Y-%m-%d %T.%e] [%l] [%s:%#] %v");
}

void app::create_postgres_connection_pool()
{
    try
    {
        std::string const host = m_config_manager->database_host();
        uint16_t const port = m_config_manager->database_port();
        std::string const username = m_config_manager->database_username();
        std::string const password = m_config_manager->database_password();
        std::string const name = m_config_manager->database_name();
        uint16_t const connection_timeout =
            m_config_manager->database_connection_timeout();
        uint16_t const pool_size =
            m_config_manager->database_connection_pool_size();

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

        m_connection_pool = std::make_unique<connection_pool<pqxx_ptr>>
        (
            std::move(connections)
        );
    }
    catch (std::exception const& e)
    {
        SPDLOG_CRITICAL
        (
            "Error in create_postgres_connection_pool: {}",
            e.what()
        );
        std::exit(EXIT_FAILURE);
    }
}

int app::main()
{
    // Create data store registry to inject into service registry.
    m_store_registry = std::make_unique<store_registry>(m_connection_pool.get());

    // Create service registry to inject into the router.
    m_service_registry = std::make_unique<service_registry>
    (
        m_store_registry.get()
    );

    // Create router to inject into the handler factory.
    m_router = std::make_unique<router>(m_service_registry.get());

    // Create and start the HTTP server.
    try
    {
        // The io_context is required for all I/O.
        net::thread_pool ioc(m_config_manager->server_threads());

        // Create and launch a listening port.
        std::make_shared<listener>
        (
            ioc.get_executor(),
            tcp::endpoint
            {
                net::ip::make_address(m_config_manager->server_host()),
                m_config_manager->server_port()
            },
            std::move(m_router)
        )->run();

        SPDLOG_INFO
        (
            "Started {} v{} on port {}",
            m_config_manager->app_domain() + "." + m_config_manager->app_name(),
            m_config_manager->app_version(),
            m_config_manager->server_port()
        );

        // Capture SIGINT and SIGTERM to perform a clean shutdown.
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait
        (
            [&ioc](beast::error_code const&, int)
            {
                SPDLOG_INFO("Stopping I/O context.");
                ioc.stop();
            }
        );

        ioc.join();
    }
    catch (beast::system_error const& e)
    {
        SPDLOG_CRITICAL("Server error: {}", e.what());
        std::exit(EXIT_FAILURE);
    }

    SPDLOG_INFO("Shutting down.");

    return EXIT_SUCCESS;
}
