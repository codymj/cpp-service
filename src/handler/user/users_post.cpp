#include "users_post.hpp"
#include "password_crypt.hpp"
#include <iostream>

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
    std::string buffer(len, 0);
    istream.read(buffer.data(), len);

    // Parse JSON.
    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string(buffer);
    auto doc = parser.iterate(json);
    std::string_view tmp{};

    doc["email"].get(tmp);
    std::string email{tmp.data(), tmp.length()};

    doc["password"].get(tmp);
    std::string password{tmp.data(), tmp.length()};
    std::string hashedPassword = PasswordCrypt::hashPassword(password);

    doc["firstName"].get(tmp);
    std::string firstName{tmp.data(), tmp.length()};

    doc["lastName"].get(tmp);
    std::string lastName{tmp.data(), tmp.length()};

    // Create user object to save.
    User userToSave
    {
        email,
        hashedPassword,
        firstName,
        lastName
    };

    // Pass to service.
    m_userService->saveUser(userToSave);

    // Send response.
    res.setContentType("application/json");
    res.setStatus(HTTPResponse::HTTP_OK);
    std::ostream &os = res.send();
    os << "";
}
