#include "app.hpp"
#include "handler_factory.hpp"
#include <Poco/Environment.h>
#include <iostream>

void
App::initialize(Application&)
{
    loadConfiguration("app.properties");
}

void
App::uninitialize()
{
    ServerApplication::uninitialize();
}

ConnectionPool<PostgresConnectionPtr>*
App::createDatabaseConnectionPool()
{
    // Get environment variable database password.
    std::string password = Poco::Environment::get
    (
        config().getString("database.password")
    );

    std::string host{}, username{}, name{};
    int port{}, connTimeout{}, poolSize{};
    try
    {
        host = config().getString("database.host");
        port = config().getInt("database.port");
        username = config().getString("database.username");
        name = config().getString("database.name");
        connTimeout = config().getInt("database.connection_timeout");
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
    std::cout << "host : " << host << '\n';
    std::cout << "port : " << port << '\n';
    std::cout << "username : " << username << '\n';
    std::cout << "connTimeout : " << connTimeout << '\n';

    // Build database connections.
    std::vector<PostgresConnectionPtr> connections;
    auto cxn = PostgresConnection
    (
        host, port, username, password, name, connTimeout
    );
    for (auto i=0; i<poolSize; ++i)
    {
        std::cout << "building connection : " << i << '\n';
        connections.emplace_back(cxn.build());
    }

    return new ConnectionPool<PostgresConnectionPtr>(std::move(connections));
}

int
App::main(const std::vector<std::string>&)
{
    auto port = config().getInt("server.port", 9000);
    ServerSocket ss(port);

    // Initialize database connection pool.
    auto connectionPool = createDatabaseConnectionPool();

    // Create registries and inject into router.
    auto storeRegistry = std::make_unique<StoreRegistry>(connectionPool);
    auto serviceRegistry = std::make_unique<ServiceRegistry>
    (
        std::move(storeRegistry)
    );
    auto router = std::make_unique<Router>(std::move(serviceRegistry));

    // Create and start the HTTP server.
    HTTPServer server
    (
        new HandlerFactory(std::move(router)),
        ss,
        new HTTPServerParams
    );
    server.start();

    // Waiting for interrupts.
    waitForTerminationRequest();
    server.stop();
    delete connectionPool;

    return Application::EXIT_OK;
}
