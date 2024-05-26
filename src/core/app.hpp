#pragma once

#include "handler_factory.hpp"
#include <postgres_connection.hpp>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <memory>

using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;

class App
: public Poco::Util::ServerApplication
{
protected:
    /**
     * Initializes application.
     * @param self
     */
    [[maybe_unused]] void
    initialize(Application& self) override;

    /**
     * Uninitializes application.
     */
    [[maybe_unused]] void
    uninitialize() override;

    /**
     * Main application loop for the HTTP server.
     * @param args
     * @return Exit code.
     */
    int
    main(const std::vector<std::string>& args) override;

private:
    /**
     * Creates the connection pool for the database with parameters from
     * the app.properties file. The pool gets injected into the StoreRegistry.
     * @return Shared pointer of the connection pool.
     */
    void
    createPostgresConnectionPool();

    std::unique_ptr<ConnectionPool<PqxxPtr>> m_connectionPool;
    std::unique_ptr<StoreRegistry> m_storeRegistry;
    std::unique_ptr<ServiceRegistry> m_serviceRegistry;
    std::unique_ptr<Router> m_router;
    uint16_t m_serverPort;
};
