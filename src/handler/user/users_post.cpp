#include "users_post.hpp"
#include "util/json_marshaller.hpp"
#include <iostream>

void
UsersPostHandler::handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
{
    // TODO: JSON validation — maybe middleware?

    // Parse request body into a User object.
    User userToSave = JsonMarshaller::toUser(req);
    std::cout << userToSave.getEmail() << '\n';

    // Pass to service.
    m_userService->saveUser(userToSave);

    // Send response.
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_OK);
    std::ostream &os = res.send();
    os << "";
}