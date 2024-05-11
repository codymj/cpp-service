#include "service_registry.hpp"

std::shared_ptr<UserService> ServiceRegistry::getUserService() {
    return m_userService;
}
