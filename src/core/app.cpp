#include "app.hpp"
#include "handler_factory.hpp"
#include <Poco/Environment.h>
#include <iostream>

void App::initialize(Application&) {
    loadConfiguration("app.properties");
}

void App::uninitialize() {
    ServerApplication::uninitialize();
}

std::shared_ptr<ConnectionPool> App::createDatabaseConnectionPool() {
    // Get environment variable database password.
    std::string password{};
    try {
        password = Poco::Environment::get(
            config().getString("database.password")
        );
    } catch (Poco::NotFoundException& nfe) {
        // TODO: When logging is implemented.
        std::cerr << nfe.what() << '\n';
        EXIT_FAILURE;
    }

    return std::make_shared<ConnectionPool>(
        config().getString("database.host"),
        config().getInt("database.port"),
        config().getString("database.username"),
        password,
        config().getString("database.name"),
        config().getInt("database.connection_timeout"),
        config().getInt("database.connection_pool_size")
    );
}

int App::main(const std::vector<std::string>&) {
    auto port = config().getInt("server.port", 9000);
    ServerSocket ss(port);

    // Initialize database connection pool.
    auto connectionPool = createDatabaseConnectionPool();

    // Create registries and inject into router.
    auto storeRegistry = std::make_unique<StoreRegistry>(connectionPool);
    auto serviceRegistry = std::make_unique<ServiceRegistry>(
        std::move(storeRegistry)
    );
    auto router = std::make_unique<Router>(std::move(serviceRegistry));

    // Create and start the HTTP server.
    HTTPServer server(
        new HandlerFactory(std::move(router)),
        ss,
        new HTTPServerParams
    );
    server.start();

    waitForTerminationRequest();
    server.stop();

    return Application::EXIT_OK;
}
