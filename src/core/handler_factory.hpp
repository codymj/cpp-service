#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include "router.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

/**
 * Subclass of Poco::Net::HTTPRequestHandlerFactory to add some customization
 * such as injecting an HTTP request router.
 */
class HandlerFactory final
: public HTTPRequestHandlerFactory
{
public:
    /**
     * Initializer for the HTTP handler factory.
     * @param router HTTP router.
     */
    explicit HandlerFactory(Router* router)
    : m_router(router)
    {};

    /**
     * Generates a RouteKey from the HTTPServerRequest which is used in m_router
     * to get the appropriate HTTPRequestHandler*.
     * @param req HTTP request into the server.
     * @return HTTPRequestHandler* to handle the request.
     */
    HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& req) override;

private:
    /**
     * Used to route HTTP requests to the appropriate handler.
     */
    Router* m_router{};
};
