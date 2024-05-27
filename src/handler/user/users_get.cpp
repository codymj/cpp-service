#include "users_get.hpp"
#include "util/json_marshaller.hpp"

void
UsersGetHandler::handleRequest
(
    HTTPServerRequest& /*req*/,
    HTTPServerResponse &res
)
{
    // Call to service to get some data.
    std::unique_ptr<std::vector<User>> users = m_userService->getUsers();

    // Marshal users to JSON.
    nlohmann::json json = JsonMarshaller::toJson(*users);

    res.setChunkedTransferEncoding(true);
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_OK);

    std::ostream &os = res.send();
    os << json;
}
