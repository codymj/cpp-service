#include "users_get.hpp"

void UsersGetHandler::handleRequest(HTTPServerRequest & /*req*/, HTTPServerResponse &res) {
    // Call to service to get some data.
    std::unique_ptr<std::vector<User>> users = m_userService->getUsers();

    res.setChunkedTransferEncoding(true);
    res.setContentType("text/html");
    res.setStatus(HTTPResponse::HTTP_OK);

    std::ostream &os = res.send();
    os << "<html>";
    os << "<h1>GET /users</h1>";

    if (!users) {
        os << "<h3>Error getting users.</h3>";
        return;
    }
    for (auto const &u: *users) {
        os << "<h3>" << u.getUserId() << "</h3>";
        os << "<ul>";
        os << "<li>" << u.getFirstName() << "</li>";
        os << "<li>" << u.getLastName() << "</li>";
        os << "<li>" << u.getEmail() << "</li>";
        os << "<li>" << u.getCreatedAt() << "</li>";
        os << "<li>" << u.getModifiedAt() << "</li>";
        os << "</ul>";
    }
    os << "</html>";
}
