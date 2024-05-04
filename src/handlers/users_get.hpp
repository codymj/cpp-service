#pragma once

#include <iostream>
#include <memory>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "core/router.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class UsersGetHandler : public HTTPRequestHandler {
public:
    /**
     * Handler for GET /users.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;
};
