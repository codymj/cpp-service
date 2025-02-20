#include "router.hpp"
#include "../handler/middleware/logger.hpp"
#include "../handler/misc/not_found.hpp"
#include "../handler/user/users_get.hpp"
#include "../handler/user/users_post.hpp"
#include <memory>

void router::create_routes()
{
    create_user_routes();
}

std::unique_ptr<handler> router::lookup_handler(route_key const& key) const
{
    if (auto it = m_routes.find(key); it == m_routes.end()) {
        return std::make_unique<not_found_handler>();
    }
    else {
        return (it->second)();
    }
}

void router::create_user_routes()
{
    auto with_logging = [this]<typename T>() -> handler_factory {
        return [this]() -> std::unique_ptr<handler> {
            return std::make_unique<logger_middleware>(
                std::make_unique<T>(m_service_registry->get_user_service())
            );
        };
    };

    m_routes.emplace(
        route_key{http::verb::get, "/users"},
        with_logging.operator()<class users_get_handler>()
    );
    m_routes.emplace(
        route_key{http::verb::post, "/users"},
        with_logging.operator()<class users_post_handler>()
    );
}
