#include "router.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"

HTTPRequestHandler*
Router::lookupHandler(RouteKey const& key)
{
    NewHandlerFunc f = m_routes[key];
    if (!f)
    {
        return new NotFoundHandler();
    }
    return f();
}

void
Router::createRoutes()
{
    // Add all HTTP routes here.
    m_routes = {
        {
            RouteKey{HTTPRequest::HTTP_GET, "/users"},
            [&]() -> HTTPRequestHandler*
            {
                return new UsersGetHandler(m_serviceRegistry->getUserService());
            }
        }
    };
}
