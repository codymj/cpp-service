#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>

using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

class HelloRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& req, HTTPServerResponse& res) override;

private:
    void static handleGet(HTTPServerRequest &req, HTTPServerResponse &res);
};
