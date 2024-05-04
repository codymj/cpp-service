#include "router.hpp"
#include "../handlers/users_get.hpp"

Router::Router() {
    m_routes = {
        {
            RouteKey{HTTPRequest::HTTP_GET, "/users"},
            []() -> HTTPRequestHandler* {return new UsersGetHandler();}
        }
    };
}

HTTPRequestHandler* Router::lookupHandler(RouteKey const& key) {
    NewHandlerFunc f = m_routes[key];
    return f();
}
