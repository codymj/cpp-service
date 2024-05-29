#pragma once

#include "../../core/router.hpp"
#include "../../service/user/user_service.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <memory>
#include <utility>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

/**
 * Handler for parsing HTTP GET /users requests.
 */
class UsersGetHandler
: public HTTPRequestHandler
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    UsersGetHandler() = delete;
    UsersGetHandler(UsersGetHandler&) = delete;
    UsersGetHandler(UsersGetHandler&&) = delete;

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
    void
    handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Service layer for User.
     */
    UserService* m_userService;

    /**
     * Next handler in the chain, if any.
     */
    HTTPRequestHandler* m_nextHandler = nullptr;
};
