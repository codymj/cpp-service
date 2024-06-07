#include "app.hpp"
#include "handler_factory.hpp"
#include <Poco/Environment.h>
#include <exception>
#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>

void App::initialize(Application&)
{
    loadConfiguration("app.properties");
    initLogger();
    initAppInfo();
    createPostgresConnectionPool();
    ServerApplication::initialize(*this);
}

void App::uninitialize()
{
    ServerApplication::uninitialize();
}

void App::initAppInfo()
{
    try
    {
        m_serverPort = config().getInt("server.port", 9000);
        m_appDomain = config().getString("app.domain");
        m_appName = config().getString("app.name");
        m_appVersion = config().getString("app.version");
    }
    catch (Poco::NotFoundException& e)
    {
        m_logger->critical("{}: {}", e.what(), e.message());
        std::exit(EXIT_CONFIG);
    }
    catch (std::exception& e)
    {
        m_logger->critical("Error in createPostgresConnectionPool: {}", e.what());
        std::exit(EXIT_CONFIG);
    }
}

void App::initLogger()
{
    try
    {
        std::string const level = config().getString("app.log_level");
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

    m_logger = spdlog::stdout_color_mt("logger");
}

void App::createPostgresConnectionPool()
{
    try
    {
        // Get environment variable database password.
        std::string const password = Poco::Environment::get
        (
            config().getString("database.password")
        );

        // Get the rest of the configuration parameters.
        std::string const host = config().getString("database.host");
        uint16_t const port = config().getInt("database.port");
        std::string const username = config().getString("database.username");
        std::string const name = config().getString("database.name");
        uint16_t const connectionTimeout = config().getInt("database.connection_timeout");
        uint16_t const poolSize = config().getInt("database.connection_pool_size");

        // Build database connections.
        std::vector<PqxxPtr> connections;
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
        m_logger->critical("{}: {}", e.what(), e.message());
        std::exit(EXIT_CONFIG);
    }
    catch (std::exception& e)
    {
        m_logger->critical("Error in createPostgresConnectionPool: {}", e.what());
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
    ServerSocket const serverSocket(m_serverPort);
    HTTPServer server
    (
        new HandlerFactory(m_router.get()),
        serverSocket,
        new HTTPServerParams
    );

    m_logger->info("Starting server on port {}", m_serverPort);
    server.start();

    // Waiting for interrupts.
    waitForTerminationRequest();

    m_logger->info("Shutting down server...", m_serverPort);
    server.stop();

    return EXIT_OK;
}
