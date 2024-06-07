#include "router.hpp"
#include "../handler/middleware/logger.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"
#include "../handler/user/users_post.hpp"

void Router::createRoutes()
{
    createUserRoutes();
}

HTTPRequestHandler* Router::lookupHandler(RouteKey const& key)
{
    NewHandlerFunc const f = m_routes[key];
    if (!f)
    {
        return new NotFoundHandler();
    }
    return f();
}

void Router::createUserRoutes()
{
    using Poco::Net::HTTPRequest;
    
    m_routes.insert
    ({
        RouteKey{HTTPRequest::HTTP_GET, "/users"},
        [&]() -> HTTPRequestHandler*
        {
            // Initialize handlers.
            auto const usersGetHandler = new UsersGetHandler
            (
                m_serviceRegistry->getUserService()
            );
            auto const loggerMiddleware = new LoggerMiddleware(usersGetHandler);

            // Return first handler in chain.
            return loggerMiddleware;
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
