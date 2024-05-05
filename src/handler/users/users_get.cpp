#include "users_get.hpp"

void UsersGetHandler::handleRequest(HTTPServerRequest& /*req*/, HTTPServerResponse& res) {
    // Call to service to get some data.
    std::vector<UserModel> users = m_userService->getUsers();

    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");
    res.setStatus(HTTPResponse::HTTP_OK);

    std::ostream& os = res.send();
    os << "<h1>GET /users</h1>";
    os << "<ul>Users";
    os << "<li>" << users[0].getId() << "</li>";
    os << "<li>" << users[1].getId() << "</li>";
    os << "<li>" << users[2].getId() << "</li>";
    os << "</ul>";
}
