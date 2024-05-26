#pragma once

#include "../../core/router.hpp"
#include "../../service/user/user_service.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <simdjson.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class UsersPostHandler
: public HTTPRequestHandler
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    UsersPostHandler() = delete;
    UsersPostHandler(UsersPostHandler&) = delete;
    UsersPostHandler(UsersPostHandler&&) = delete;

    explicit UsersPostHandler(UserService* userService)
    : m_userService(userService)
    {}

    /**
     * Handler for GET /users.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void
    handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Service layer for User.
     */
    UserService* m_userService;
};
