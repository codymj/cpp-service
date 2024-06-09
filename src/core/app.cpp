#include "app.hpp"
#include "handler_factory.hpp"
#include <config_manager.hpp>
#include <Poco/Environment.h>
#include <Poco/Net/HTTPServer.h>
#include <exception>
#include <iostream>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

void App::initialize(Application&)
{
    ConfigManager::instance().initialize("app.properties");
    ServerApplication::initialize(*this);

    initLogger();
    createPostgresConnectionPool();
}

void App::uninitialize()
{
    ServerApplication::uninitialize();
}

void App::initLogger()
{
    try
    {
        std::string const level =
            ConfigManager::instance().config().getString("app.log_level");
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
        std::cerr << "Error loading property app.log_level: " << e.what() << '\n';
        std::exit(EXIT_CONFIG);
    }

    spdlog::init_thread_pool(8192, 1);
    auto const console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto const m_logger = std::make_shared<spdlog::async_logger>(
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
        // Get environment variable database password.
        std::string const password = Poco::Environment::get
        (
            ConfigManager::instance().config().getString("database.password")
        );

        // Get the rest of the configuration parameters.
        std::string const host =
            ConfigManager::instance().config().getString("database.host");
        uint16_t const port =
            ConfigManager::instance().config().getInt("database.port");
        std::string const username =
            ConfigManager::instance().config().getString("database.username");
        std::string const name =
            ConfigManager::instance().config().getString("database.name");
        uint16_t const connectionTimeout =
            ConfigManager::instance().config().getInt("database.connection_timeout");
        uint16_t const poolSize =
            ConfigManager::instance().config().getInt("database.connection_pool_size");

        // Build database connections.
        std::vector<PqxxPtr> connections;
        connections.reserve(poolSize);

        auto const cxn = PostgresConnection
        (
            host, port, username, password, name, connectionTimeout
        );
        for (auto i=0; i<poolSize; ++i)
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
    auto const serverSocket(ConfigManager::instance().config().getInt("server.port"));

    auto const serverParams = new HTTPServerParams();
    std::string const serverName
    {
        ConfigManager::instance().config().getString("app.domain") +
        "." +
        ConfigManager::instance().config().getString("app.name")
    };
    serverParams->setServerName(serverName);
    serverParams->setSoftwareVersion
    (
        ConfigManager::instance().config().getString("app.version")
    );
    serverParams->setTimeout
    (
        Poco::Timespan(ConfigManager::instance().config().getInt("server.timeout"), 0)
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
        ConfigManager::instance().config().getString("app.version"),
        ConfigManager::instance().config().getInt("server.port")
    );
    server.start();

    // Waiting for interrupts.
    waitForTerminationRequest();

    SPDLOG_INFO("Shutting down server...");
    server.stop();

    return EXIT_OK;
}
