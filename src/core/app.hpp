#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <memory>
#include "postgres_connection.hpp"
#include "store_registry.hpp"

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
    void
    initialize(Application& self) override;

    /**
     * Uninitializes application (not useful yet).
     */
    void
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
    ConnectionPool<PostgresConnectionPtr>*
    createDatabaseConnectionPool();
};
