#include "router.hpp"
#include "../handler/middleware/logger.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"
#include "../handler/user/users_post.hpp"

void router::create_routes()
{
    create_user_routes();
}

std::unique_ptr<handler> router::lookup_handler(route_key const& key)
{
    handler_func const f = m_routes[key];
    if (!f)
    {
        return std::make_unique<not_found_handler>();
    }

    return f();
}

void router::create_user_routes()
{
    m_routes.insert
    ({
        route_key{http::verb::get, "/users"},
        [&]() -> std::unique_ptr<handler>
        {
            auto ugh = std::make_unique<users_get_handler>
            (
                m_service_registry->get_user_service()
            );
            return std::make_unique<logger_middleware>(std::move(ugh));
        }
    });
    m_routes.insert
    ({
        route_key{http::verb::post, "/users"},
        [&]() -> std::unique_ptr<handler>
        {
            auto uph = std::make_unique<users_post_handler>
            (
                m_service_registry->get_user_service()
            );
            return std::make_unique<logger_middleware>(std::move(uph));
        }
    });
}
