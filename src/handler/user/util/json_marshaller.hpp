#pragma once

#include "../../../store/user/user_model.hpp"
#include "password_crypt.hpp"
#include <Poco/Net/HTTPServerRequest.h>
#include <nlohmann/json.hpp>

using Poco::Net::HTTPServerRequest;

class JsonMarshaller
{
public:
    /**
     * Parses the request body into a User object.
     * @param req is the incoming HTTPServerRequest.
     * @return User object.
     */
    static User
    toUser(HTTPServerRequest& req)
    {
        // Parse request body.
        auto& istream = req.stream();
        int64_t const len = req.getContentLength();
        std::string buffer(len, 0);
        istream.read(buffer.data(), len);

        // Parse JSON.
        nlohmann::json json = nlohmann::json::parse(buffer);
        std::string email{json["email"]};
        std::string password{json["password"]};
        std::string hashedPassword = PasswordCrypt::hashPassword(password);
        std::string firstName{json["firstName"]};
        std::string lastName{json["lastName"]};

        // Create user object to save.
        return User{email, hashedPassword, firstName, lastName};
    }

    /**
     * Marshals a single User to JSON.
     * @param user to marshal into JSON.
     * @return JSON object of the user.
     */
    static nlohmann::json
    toJson(User const& user)
    {
        nlohmann::json json{};
        json["userId"] = user.getUserId();
        json["email"] = user.getEmail();
        json["firstName"] = user.getFirstName();
        json["lastName"] = user.getLastName();
        json["createdAt"] = user.getCreatedAt();
        json["modifiedAt"] = user.getModifiedAt();

        return json;
    }

    /**
     * Marshals a collection of users to JSON.
     * @param users to marshal into JSON.
     * @return JSON array of user objects.
     */
    static nlohmann::json
    toJson(std::vector<User> const& users)
    {
        nlohmann::json json{};
        for (auto const& u : users)
        {
            json.emplace_back(toJson(u));
        }

        return json;
    }
};
