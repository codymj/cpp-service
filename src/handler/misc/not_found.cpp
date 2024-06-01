#include "not_found.hpp"

void NotFoundHandler::handleRequest
(
    HTTPServerRequest& req,
    HTTPServerResponse& res
)
{
    res.setChunkedTransferEncoding(true);
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_NOT_FOUND);

    std::ostream& os = res.send();
    os << "";

    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
