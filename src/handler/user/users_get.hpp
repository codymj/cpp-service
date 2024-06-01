#pragma once

#include "../../service/user/user_service.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

/**
 * Handler for parsing HTTP GET /users requests.
 */
class UsersGetHandler final
: public HTTPRequestHandler
{
public:
    /**
     * Handler for GET /users
     * @param userService User service layer.
     * @param next Next handler in chain.
     */
    explicit UsersGetHandler
    (
        UserService* userService,
        HTTPRequestHandler* next = nullptr
    )
    : m_userService(userService)
    , m_nextHandler(next)
    {}

    /**
     * Destructor to clean up chained handlers.
     */
    ~UsersGetHandler() override
    {
        delete m_nextHandler;
    }

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
