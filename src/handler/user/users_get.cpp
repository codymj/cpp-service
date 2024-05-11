#include "users_get.hpp"

void UsersGetHandler::handleRequest(HTTPServerRequest& /*req*/, HTTPServerResponse& res) {
    // Call to service to get some data.
    std::unique_ptr<std::vector<User>> users = m_userService->getUsers();

    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");
    res.setStatus(HTTPResponse::HTTP_OK);

    std::ostream& os = res.send();
    os << "<h1>GET /users</h1>";
    os << "<ul>Users";
    for (auto const& u : *users) {
        os << "<li>" << "UserID: " << u.getUserId() << "</li>";
    }
    os << "</ul>";
}
