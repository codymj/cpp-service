#include "logger.hpp"
#include <iostream>

void LoggerMiddleware::handleRequest
(
    HTTPServerRequest& req,
    HTTPServerResponse& res
)
{
    // [For testing purposes]
    // TODO: logging
    std::cout << "Client Address: " << req.clientAddress() << "\t"
    << req.getMethod() << " " << req.getURI() << '\n';

    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
