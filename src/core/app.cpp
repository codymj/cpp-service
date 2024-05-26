#include "app.hpp"
#include <Poco/Environment.h>
#include <iostream>

[[maybe_unused]] void
App::initialize(Application&)
{
    loadConfiguration("app.properties");
    m_serverPort = config().getInt("server.port", 9000);

    createPostgresConnectionPool();
}

[[maybe_unused]] void
App::uninitialize()
{
    ServerApplication::uninitialize();
}

void
App::createPostgresConnectionPool()
{
    // Get environment variable database password.
    std::string password = Poco::Environment::get
    (
        config().getString("database.password")
    );

    std::string host{}, username{}, name{};
    uint16_t port{}, connectionTimeout{}, poolSize{};
    try
    {
        host = config().getString("database.host");
        port = config().getInt("database.port");
        username = config().getString("database.username");
        name = config().getString("database.name");
        connectionTimeout = config().getInt("database.connection_timeout");
        poolSize = config().getInt("database.connection_pool_size");
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

int
App::main(const std::vector<std::string>&)
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
    ServerSocket serverSocket(m_serverPort);
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

    return Application::EXIT_OK;
}
