#pragma once

#include "../../../include/chained_handler.hpp"
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
: public ChainedHandler
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    UsersGetHandler() = delete;
    UsersGetHandler(UsersGetHandler&) = delete;
    UsersGetHandler(UsersGetHandler&&) = delete;

    explicit UsersGetHandler(UserService* userService)
    : m_userService(userService)
    {}

    ~UsersGetHandler() override
    {
        delete m_nextHandler;
    }

    /**
     * Set next handler.
     * @param handler is the next handler in the chain.
     */
    void
    setNextHandler(ChainedHandler* handler) override;

    /**
     * Handler for GET /users.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void
    handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Next handler in the chain, if any.
     */
    ChainedHandler* m_nextHandler = nullptr;

    /**
     * Service layer for User.
     */
    UserService* m_userService;
};
