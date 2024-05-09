#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include "router.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class HandlerFactory : public HTTPRequestHandlerFactory {
public:
    HandlerFactory();

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
    std::unique_ptr<Router> m_router;
};
