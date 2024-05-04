#pragma once

#include "core/router.hpp"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class NotFoundHandler : public HTTPRequestHandler {
public:
    /**
     * Handler to return 404 error.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;
};
