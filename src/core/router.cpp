#include "router.hpp"
#include <Poco/Net/HTTPRequest.h>
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
            auto const ugh = new UsersGetHandler(m_serviceRegistry->getUserService());
            return new LoggerMiddleware(ugh);
        }
    });
    m_routes.insert
    ({
        RouteKey{HTTPRequest::HTTP_POST, "/users"},
        [&]() -> HTTPRequestHandler*
        {
            auto const uph = new UsersPostHandler(m_serviceRegistry->getUserService());
            return new LoggerMiddleware(uph);
        }
    });
}
