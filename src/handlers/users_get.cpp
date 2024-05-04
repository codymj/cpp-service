#include "users_get.hpp"

void UsersGetHandler::handleRequest(HTTPServerRequest& /*req*/, HTTPServerResponse& res) {
    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");

    std::ostream& os = res.send();
    os << "<h1>GET /users</h1>";
}
