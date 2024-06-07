#include "logger.hpp"
#include <Poco/Net/HTTPServerRequest.h>

void LoggerMiddleware::handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
{
    // Log request.
    m_logger->info("{} {} from {}", req.getMethod(), req.getURI(), req.clientAddress().toString());

    // Call next handler, if any.
    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
