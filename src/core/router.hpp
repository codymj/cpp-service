#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <functional>
#include <handler.hpp>
#include <map>
#include "service_registry.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

using route_key = std::pair<http::verb, std::string>;
using handler_func = std::function<std::unique_ptr<handler>()>;

/**
 * The router acts as a typical HTTP request router which routes requests by
 * HTTP method and path to the appropriate handler. The Router gets injected
 * into the HandlerFactory.
 */
class router
{
public:
    router(router const&) = delete;
    router& operator=(router const&) = delete;
    router(router&&) noexcept = delete;
    router& operator=(router&&) noexcept = delete;

    /**
     * Initializer for the HTTP router.
     * @param service_registry Registry for business services.
     */
    explicit router(ServiceRegistry* service_registry)
    : m_service_registry(service_registry)
    {
        create_routes();
    }

    /**
     * Gets the appropriate NewHandlerFunc from m_routes by RouteKey and then
     * calls that function to return the provided HTTPRequestHandler*.
     * @return HTTPRequestHandler* to handle the request.
     */
    std::unique_ptr<handler> lookup_handler(route_key const& key);

private:

    /**
     * Calls the helper functions to insert HTTP routes into m_routes map.
     */
    void create_routes();

    /**
     * Adds User HTTP routes to the m_routes map.
     */
     void create_user_routes();

    /**
     * A map which uses a RouteKey as the key to a function that returns a new
     * Poco::Net::HTTPRequestHandler*. Since Poco deletes the handlers, this
     * function acts as an HTTPRequestHandler* factory.
     */
    std::map<route_key, handler_func> m_routes{};

    /**
     * Registry which contains all services to perform business logic. Its
     * various getter methods are called to inject appropriate service objects
     * into handlers.
     */
    ServiceRegistry* m_service_registry{};
};
