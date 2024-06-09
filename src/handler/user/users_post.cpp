#include "users_post.hpp"
#include "util/json_marshaller.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

void UsersPostHandler::handleRequest(HTTPServerRequest& req, HTTPServerResponse& res)
{
    // TODO: JSON validation middleware.

    try
    {
        // Parse request body into a User object.
        User const userToSave = JsonMarshaller::toUser(req);

        // Pass to service.
        m_userService->saveUser(userToSave);
    }
    catch (std::runtime_error const& e)
    {
        SPDLOG_ERROR("{}", e.what());
        res.setContentType("application/json");
        res.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        std::ostream &os = res.send();
        os << "";
        return;
    }

    // Send response.
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_OK);
    std::ostream &os = res.send();
    os << "";

    // Call next handler.
    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
