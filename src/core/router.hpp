#pragma once

#include "service_registry.hpp"
#include <functional>
#include <map>
#include <string>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;

using HttpMethod = std::string const;
using RoutePath = std::string const;
using RouteKey = std::pair<HttpMethod, RoutePath>;
using NewHandlerFunc = std::function<HTTPRequestHandler*()>;

/**
 * The router acts as a typical HTTP request router which routes requests by
 * HTTP method and path to the appropriate handler. The Router gets injected
 * into the HandlerFactory.
 */
class Router
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    Router() = delete;
    Router(Router&) = delete;
    Router(Router&&) = delete;

    explicit Router(ServiceRegistry* serviceRegistry)
    : m_serviceRegistry(serviceRegistry)
    {
        createRoutes();
    };

    /**
     * Gets the appropriate NewHandlerFunc from m_routes by RouteKey and then
     * calls that function to return the provided HTTPRequestHandler*.
     * @return HTTPRequestHandler* to handle the request.
     */
    HTTPRequestHandler*
    lookupHandler(RouteKey const&);

private:
    /**
     * Calls the helper functions to insert HTTP routes into m_routes map.
     */
    void
    createRoutes();

    /**
     * Adds User HTTP routes to the m_routes map.
     */
     void
     createUserRoutes();

    /**
     * A map which uses a RouteKey as the key to a function that returns a new
     * Poco::Net::HTTPRequestHandler*. Since Poco deletes the handlers, this
     * function acts as an HTTPRequestHandler* factory.
     */
    std::map<RouteKey,NewHandlerFunc> m_routes{};

    /**
     * Registry which contains all services to perform business logic. Its
     * various getter methods are called to inject appropriate service objects
     * into handlers.
     */
    ServiceRegistry* m_serviceRegistry{};
};
