#pragma once

#include "router.hpp"
#include "service_registry.hpp"
#include "store_registry.hpp"
#include <postgres_connection.hpp>
#include <Poco/Util/ServerApplication.h>
#include <memory>

using Poco::Util::ServerApplication;

class App final
: public ServerApplication
{
protected:
    /**
     * Initializes application.
     * @param self
     */
    void initialize(Application& self) override;

    /**
     * Uninitializes application.
     */
    void uninitialize() override;

    /**
     * Main application loop for the HTTP server.
     * @param args
     * @return Exit code.
     */
    int main(const std::vector<std::string>& args) override;

private:
    /**
     * Initializes logger.
     */
    static void initLogger();

    /**
     * Creates the connection pool for the database with parameters from
     * the app.properties file. The pool gets injected into the StoreRegistry.
     * @return Shared pointer of the connection pool.
     */
    void createPostgresConnectionPool();

    std::unique_ptr<ConnectionPool<PqxxPtr>> m_connectionPool;
    std::unique_ptr<StoreRegistry> m_storeRegistry;
    std::unique_ptr<ServiceRegistry> m_serviceRegistry;
    std::unique_ptr<Router> m_router;
};
