#pragma once

#include "service_registry.hpp"
#include <functional>
#include <map>
#include <utility>
#include <string>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;


// TODO: Clean this up into a namespace or something.
using HttpMethod = std::string const;
using RoutePath = std::string const;
using RouteKey = std::pair<HttpMethod, RoutePath>;
using NewHandlerFunc = std::function<HTTPRequestHandler*()>;

class Router {
public:
    Router() = delete;
    Router(Router&) = delete;
    explicit Router(std::unique_ptr<ServiceRegistry> serviceRegistry)
    : m_serviceRegistry(std::move(serviceRegistry)) {
        createRoutes();
    };

    /**
     * Gets the appropriate NewHandlerFunc from m_routes by RouteKey and then
     * calls that function to return the provided HTTPRequestHandler*.
     * @return HTTPRequestHandler* to handle the request.
     */
    HTTPRequestHandler* lookupHandler(RouteKey const&);

private:
    /**
     * Adds HTTP routes to the m_routes map.
     */
    void createRoutes();

    /**
     * A map which uses a RouteKey as the key to a function that returns a new
     * Poco::Net::HTTPRequestHandler*. Since Poco deletes the handlers, this
     * function acts as an HTTPRequestHandler* factory.
     */
    std::map<RouteKey,NewHandlerFunc> m_routes;

    /**
     * Registry which contains all services to perform business logic. Its
     * various getter methods are called to inject appropriate service objects
     * into handlers.
     */
    std::unique_ptr<ServiceRegistry> m_serviceRegistry;
};
