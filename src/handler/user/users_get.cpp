#include "users_get.hpp"

void UsersGetHandler::handleRequest(HTTPServerRequest& /*req*/, HTTPServerResponse& res) {
    // Call to service to get some data.
    std::vector<User> users = m_userService->getUsers();

    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");
    res.setStatus(HTTPResponse::HTTP_OK);

    std::ostream& os = res.send();
    os << "<h1>GET /users</h1>";
    os << "<ul>Users";
//    os << "<li>" << "User0 ID: " << users[0].userId << "</li>";
//    os << "<li>" << "User1 ID: " << users[1].userId << "</li>";
//    os << "<li>" << "User2 ID: " << users[2].userId << "</li>";
    os << "</ul>";
}
