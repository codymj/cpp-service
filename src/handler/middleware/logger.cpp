#include "logger.hpp"
#include <context.hpp>
#include <Poco/Net/HTTPServerRequest.h>
#include <spdlog/spdlog.h>

void LoggerMiddleware::handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
{
    // Get context from thread local storage.
    auto const ctx = Context::getContext();

    // Log request.
    SPDLOG_INFO
    (
        "{} {} {} {}",
        ctx->traceId(),
        req.getMethod(),
        req.getURI(),
        req.clientAddress().toString()
    );

    // Call next handler, if any.
    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
