#include "service_registry.hpp"

ServiceRegistry::ServiceRegistry(std::unique_ptr<StoreRegistry> storeRegistry) {
    m_userService = std::make_shared<UserService>(
        storeRegistry->getUserStore()
    );
}

std::shared_ptr<UserService> ServiceRegistry::getUserService() {
    return m_userService;
}
