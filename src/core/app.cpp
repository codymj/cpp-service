#include "app.hpp"
#include "handler_factory.hpp"
#include <config_manager.hpp>
#include <Poco/Net/HTTPServer.h>
#include <exception>
#include <iostream>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void App::initialize(Application&)
{
    m_config_manager = std::make_unique<config_manager>("properties.yml");
    ServerApplication::initialize(*this);

    initLogger();
    createPostgresConnectionPool();
}

void App::uninitialize()
{
    ServerApplication::uninitialize();
}

void App::initLogger() const
{
    try
    {
        std::string const level =
            m_config_manager->app_log_level();

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
        std::cerr
            << "Error loading property app.log_level: "
            << e.what()
            << '\n';

        std::exit(EXIT_CONFIG);
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

void App::createPostgresConnectionPool()
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
        std::vector<PqxxPtr> connections;
        connections.reserve(pool_size);

        auto const cxn = PostgresConnection
        (
            host, port, username, password, name, connection_timeout
        );
        for (auto i=0; i<pool_size; ++i)
        {
            connections.emplace_back(cxn.build());
        }

        m_connectionPool = std::make_unique<ConnectionPool<PqxxPtr>>
        (
            std::move(connections)
        );
    }
    catch (Poco::NotFoundException& e)
    {
        SPDLOG_CRITICAL("{}: {}", e.what(), e.message());
        std::exit(EXIT_CONFIG);
    }
    catch (std::exception& e)
    {
        SPDLOG_CRITICAL
        (
            "Error in createPostgresConnectionPool: {}",
            e.what()
        );
        std::exit(EXIT_CONFIG);
    }
}

int App::main(const std::vector<std::string>&)
{
    using Poco::Net::HTTPServer;
    using Poco::Net::HTTPServerParams;
    using Poco::Net::ServerSocket;

    // Create data store registry to inject into service registry.
    m_storeRegistry = std::make_unique<StoreRegistry>(m_connectionPool.get());

    // Create service registry to inject into the router.
    m_serviceRegistry = std::make_unique<ServiceRegistry>
    (
        m_storeRegistry.get()
    );

    // Create router to inject into the handler factory.
    m_router = std::make_unique<Router>(m_serviceRegistry.get());

    // Create and start the HTTP server.
    auto const serverSocket(m_config_manager->server_port());

    auto const serverParams = new HTTPServerParams();
    std::string const serverName
    {
        m_config_manager->app_domain() + "." + m_config_manager->app_name()
    };
    serverParams->setServerName(serverName);
    serverParams->setSoftwareVersion(m_config_manager->app_version());
    serverParams->setTimeout
    (
        Poco::Timespan(m_config_manager->server_idle_timeout(), 0)
    );

    HTTPServer server
    (
        new HandlerFactory(m_router.get()),
        serverSocket,
        new HTTPServerParams
    );

    SPDLOG_INFO
    (
        "Starting {} v{} on port {}",
        serverName,
        m_config_manager->app_version(),
        m_config_manager->server_port()
    );
    server.start();

    // Waiting for interrupts.
    waitForTerminationRequest();

    SPDLOG_INFO("Shutting down server...");
    server.stop();

    return EXIT_OK;
}
