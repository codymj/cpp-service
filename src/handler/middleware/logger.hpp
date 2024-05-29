#pragma once

#include "../../../include/chained_handler.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class LoggerHandler
: public ChainedHandler
{
public:
    LoggerHandler() = default;

    /**
     * Don't want to copy/move this.
     */
    LoggerHandler(LoggerHandler&) = delete;
    LoggerHandler(LoggerHandler&&) = delete;

    ~LoggerHandler() override
    {
        delete m_nextHandler;
    };

    /**
     * Set next handler.
     * @param handler
     */
    void
    setNextHandler(ChainedHandler* handler) override;

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
    [[maybe_unused]] ChainedHandler* m_nextHandler = nullptr;
};
