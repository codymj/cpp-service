#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class LoggerMiddleware
: public HTTPRequestHandler
{
public:
    /**
     * Middleware for handling logging.
     * @param next
     */
    explicit LoggerMiddleware(HTTPRequestHandler* next)
    : m_nextHandler(next)
    {}

    /**
     * Destructor to clean up chained handlers.
     */
    ~LoggerMiddleware() override
    {
        delete m_nextHandler;
    };

    /**
     * Middleware handler for logging data.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Next handler in the chain, if any.
     */
    [[maybe_unused]] HTTPRequestHandler* m_nextHandler = nullptr;
};
