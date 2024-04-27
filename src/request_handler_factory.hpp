#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include "handlers/hello.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class RequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& req) override {
        if (req.getURI() == "/hello") {
            return hello_handler();
        } else {
            return nullptr;
        }
    }

private:
    static HTTPRequestHandler* hello_handler();
};
