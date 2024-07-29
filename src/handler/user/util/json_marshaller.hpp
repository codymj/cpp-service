#pragma once

#include "../../../store/user/user_model.hpp"
#include "password_crypt.hpp"
#include <nlohmann/json.hpp>

class json_marshaller
{
public:
    /**
     * Parses the request body into a User object.
     * @param req is the incoming HTTPServerRequest.
     * @return User object.
     */
    static user to_user(http::request<http::string_body> req)
    {
        // Parse JSON.
        nlohmann::json json = nlohmann::json::parse(req.body());
        std::string const email{json["email"]};
        std::string const password{json["password"]};
        std::string const hashed_password = password_crypt::hash_password(password);
        std::string const first_name{json["firstName"]};
        std::string const last_name{json["lastName"]};

        // Create user object to save.
        return user{email, hashed_password, first_name, last_name};
    }

    /**
     * Marshals a single User to JSON.
     * @param user to marshal into JSON.
     * @return JSON object of the user.
     */
    static nlohmann::json to_json(user const& user)
    {
        nlohmann::json json = nlohmann::json::object();
        json.emplace("userId", user.get_user_id());
        json.emplace("email", user.get_email());
        json.emplace("firstName", user.get_first_name());
        json.emplace("lastName", user.get_last_name());
        json.emplace("createdAt", user.get_created_at());
        json.emplace("modifiedAt", user.get_modified_at());

        return json;
    }

    /**
     * Marshals a collection of users to JSON.
     * @param users to marshal into JSON.
     * @return JSON array of user objects.
     */
    static nlohmann::json toJson(std::vector<user> const& users)
    {
        nlohmann::json json = nlohmann::json::array();
        for (auto const& u : users)
        {
            json.emplace_back(to_json(u));
        }

        return json;
    }
};
