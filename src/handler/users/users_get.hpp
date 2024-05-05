#pragma once

#include "core/router.hpp"
#include "../../service/users/user_service.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <memory>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class UsersGetHandler : public HTTPRequestHandler {
public:
    UsersGetHandler() {
        m_userService = std::make_shared<UserService>();
    }

    /**
     * Handler for GET /users.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    std::shared_ptr<UserService> m_userService;
};
