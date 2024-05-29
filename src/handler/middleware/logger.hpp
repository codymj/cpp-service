#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class LoggerHandler
: public HTTPRequestHandler
{
public:
    explicit LoggerHandler(HTTPRequestHandler* next)
    : m_nextHandler(next)
    {}

    /**
     * Don't want to copy/move this.
     */
    LoggerHandler(LoggerHandler&) = delete;
    LoggerHandler(LoggerHandler&&) = delete;

    /**
     * Destructor to clean up chained handlers.
     */
    ~LoggerHandler() override
    {
        delete m_nextHandler;
    };

    /**
     * Middleware handler for logging data.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void
    handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Next handler in the chain, if any.
     */
    [[maybe_unused]] HTTPRequestHandler* m_nextHandler = nullptr;
};
