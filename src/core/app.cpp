#include "app.hpp"
#include <Poco/Environment.h>
#include <iostream>

void App::initialize(Application&)
{
    loadConfiguration("app.properties");
    m_serverPort = config().getInt("server.port", 9000);

    createPostgresConnectionPool();
}

void App::uninitialize()
{
    ServerApplication::uninitialize();
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
        auto cxn = PostgresConnection
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
    catch (Poco::NotFoundException& nfe)
    {
        std::cerr << nfe.what() << ": " << nfe.message() << '\n';
        std::exit(EXIT_CONFIG);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::exit(EXIT_CONFIG);
    }
}

int App::main(const std::vector<std::string>&)
{
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
    server.start();

    // Waiting for interrupts.
    waitForTerminationRequest();
    server.stop();

    return EXIT_OK;
}
