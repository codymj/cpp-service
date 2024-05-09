#include "router.hpp"
#include "handler/misc/not_found.hpp"
#include "handler/user/users_get.hpp"

Router::Router() {
    // Initialize registries.
    m_storeRegistry = std::make_unique<StoreRegistry>();
    m_serviceRegistry = std::make_unique<ServiceRegistry>(
        std::move(m_storeRegistry)
    );

    // Add all HTTP routes here.
    // TODO: Maybe a way to inject these routes from the handlers?
    m_routes = {
        {
            RouteKey{HTTPRequest::HTTP_GET, "/users"},
            [&]() -> HTTPRequestHandler* {
                return new UsersGetHandler(m_serviceRegistry->getUserService());
            }
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
