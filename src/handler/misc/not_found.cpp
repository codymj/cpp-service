#include "not_found.hpp"

void
NotFoundHandler::handleRequest
(
    HTTPServerRequest& /*req*/,
    HTTPServerResponse& res
)
{
    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");
    res.setStatus(HTTPResponse::HTTP_NOT_FOUND);

    std::ostream& os = res.send();
    os << "<h1>404 Not Found</h1>";
}
