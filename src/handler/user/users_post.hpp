#pragma once

#include "../../core/router.hpp"
#include "../../service/user/user_service.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class UsersPostHandler
: public HTTPRequestHandler
{
public:
    /**
     * Handler for POST /users.
     * @param userService User service layer.
     * @param next Next handler in chain.
     */
    explicit UsersPostHandler
    (
        UserService* userService,
        HTTPRequestHandler* next = nullptr
    )
    : m_userService(userService)
    , m_nextHandler(next)
    {}

    /**
     * Handler for GET /users.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Service layer for User.
     */
    UserService* m_userService;

    /**
     * Next handler in the chain.
     */
    HTTPRequestHandler* m_nextHandler = nullptr;
};
