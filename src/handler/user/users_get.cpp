#include "users_get.hpp"
#include "util/json_marshaller.hpp"

void UsersGetHandler::handleRequest
(
    HTTPServerRequest& req,
    HTTPServerResponse& res
)
{
    // Call to service to get some data.
    std::unique_ptr<std::vector<User>> const users = m_userService->getUsers();

    // Return nothing if container is empty.
    if (!users)
    {
        res.setChunkedTransferEncoding(true);
        res.setContentType("application/json");
        res.setContentLength64(0);
        res.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        std::ostream &os = res.send();
        os << "";
        return;
    }
    if (users->empty())
    {
        res.setChunkedTransferEncoding(true);
        res.setContentType("application/json");
        res.setContentLength64(0);
        res.setStatus(HTTPResponse::HTTP_NO_CONTENT);
        std::ostream &os = res.send();
        os << "";
        return;
    }

    // Marshal users to JSON.
    nlohmann::json const json = JsonMarshaller::toJson(*users);

    // Send response.
    res.setChunkedTransferEncoding(true);
    res.setContentType("application/json");
    res.setContentLength64
    (
        static_cast<Poco::Int64>(users->size() * sizeof(users->at(0)))
    );
    res.setStatus(HTTPResponse::HTTP_OK);
    std::ostream &os = res.send();
    os << json;

    // Call next handler, if any.
    if (m_nextHandler)
    {
        m_nextHandler->handleRequest(req, res);
    }
}
