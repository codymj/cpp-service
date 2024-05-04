#pragma once

#include <functional>
#include <map>
#include <utility>
#include <string>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;

using HttpMethod = std::string const;
using RoutePath = std::string const;
using RouteKey = std::pair<HttpMethod, RoutePath>;
using NewHandlerFunc = std::function<HTTPRequestHandler*()>;

class Router {
public:
    Router();

    HTTPRequestHandler* lookupHandler(RouteKey const&);

private:
    std::map<RouteKey,NewHandlerFunc> m_routes;
};
