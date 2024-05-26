#include "users_post.hpp"

void
UsersPostHandler::handleRequest
(
    Poco::Net::HTTPServerRequest &req,
    Poco::Net::HTTPServerResponse &res
)
{
    // Parse request body.
    // TODO: add JSON validation (maybe middleware)
    auto& istream = req.stream();
    int64_t const len = req.getContentLength();
    std::string buf(len, 0);
    istream.read(buf.data(), len);

    // Parse JSON.
    simdjson::ondemand::parser parser;
    auto doc = parser.iterate(buf);

    std::string_view email{}; doc["email"].get(email);
    std::string_view password{}; doc["password"].get(password);
    std::string_view firstName{}; doc["firstName"].get(firstName);
    std::string_view lastName{}; doc["lastName"].get(lastName);

    // Create user object to save.
    User userToSave
    {
        std::string{email.data(), email.size()},
        std::string{password.data(), password.size()},
        std::string{firstName.data(), firstName.size()},
        std::string{lastName.data(), lastName.size()},
    };

    // Pass to service.
    m_userService->saveUser(userToSave);

    // Send response.
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_OK);
    std::ostream &os = res.send();
    os << "";
}
