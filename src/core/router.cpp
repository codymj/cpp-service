#include "router.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"
#include "../handler/user/users_post.hpp"

void
Router::createRoutes()
{
    createUserRoutes();
}

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
Router::createUserRoutes()
{
    m_routes.insert
    ({
        RouteKey{HTTPRequest::HTTP_GET, "/users"},
        [&]() -> HTTPRequestHandler*
        {
            return new UsersGetHandler(m_serviceRegistry->getUserService());
        }
    });
    m_routes.insert
    ({
        RouteKey{HTTPRequest::HTTP_POST, "/users"},
        [&]() -> HTTPRequestHandler*
        {
            return new UsersPostHandler(m_serviceRegistry->getUserService());
        }
    });
}
