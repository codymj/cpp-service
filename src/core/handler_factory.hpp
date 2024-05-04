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
    HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& req) override;

private:
    Router m_router{};
};
