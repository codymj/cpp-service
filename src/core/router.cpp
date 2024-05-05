#include "router.hpp"
#include "handler/misc/not_found.hpp"
#include "handler/users/users_get.hpp"

Router::Router() {
    m_routes = {
        {
            // GET /users
            RouteKey{HTTPRequest::HTTP_GET, "/users"},
            []() -> HTTPRequestHandler* {return new UsersGetHandler();}
        }
    };
}

HTTPRequestHandler* Router::lookupHandler(RouteKey const& key) {
    NewHandlerFunc f = m_routes[key];
    if (!f) {
        return new NotFoundHandler();
    }
    return f();
}
