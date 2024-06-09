#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

class NotFoundHandler final
: public HTTPRequestHandler
{
public:
    /**
     * Handler for returning 404 statuses.
     * @param next Next handler in chain.
     */
    explicit NotFoundHandler(HTTPRequestHandler* next = nullptr)
    : m_nextHandler(next)
    {}

    /**
     * Destructor to clean up chained handlers.
     */
    ~NotFoundHandler() override
    {
        delete m_nextHandler;
    }

    /**
     * Handler to return 404 error.
     * @param req HTTPServerRequest&
     * @param res HTTPServerResponse&
     */
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    /**
     * Next handler in the chain.
     */
    HTTPRequestHandler* m_nextHandler = nullptr;
};
