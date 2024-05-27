#include "router.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"
#include "../handler/user/users_post.hpp"

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
    m_routes = {
        // Users ---------------------------------------------------------------
        {
            RouteKey{HTTPRequest::HTTP_GET, "/users"},
            [&]() -> HTTPRequestHandler*
            {
                return new UsersGetHandler
                (
                    m_serviceRegistry->getUserService()
                );
            }
        },
        {
            RouteKey{HTTPRequest::HTTP_POST, "/users"},
            [&]() -> HTTPRequestHandler*
            {
                return new UsersPostHandler
                (
                    m_serviceRegistry->getUserService()
                );
            }
        }
        // End Users -----------------------------------------------------------
    };
}
