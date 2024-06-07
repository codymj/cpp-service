#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class LoggerMiddleware final
: public HTTPRequestHandler
{
public:
    /**
     * Middleware for handling logging.
     * @param next
     */
    explicit LoggerMiddleware(HTTPRequestHandler* next)
    : m_nextHandler(next)
    {
        m_logger = spdlog::get("logger");
    }

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
     * Logger for this class.
     */
    std::shared_ptr<spdlog::logger> m_logger;

    /**
     * Next handler in the chain, if any.
     */
    [[maybe_unused]] HTTPRequestHandler* m_nextHandler = nullptr;
};
