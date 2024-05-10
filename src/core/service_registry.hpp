#pragma once

#include "store_registry.hpp"
#include "../service/user/user_service.hpp"

class ServiceRegistry {
public:
    explicit ServiceRegistry(std::unique_ptr<StoreRegistry> storeRegistry);

    std::shared_ptr<UserService> getUserService();

private:
    /**
     * Service layer for User logic.
     */
    std::shared_ptr<UserService> m_userService;

    /**
     * Maintains a record of data stores, i.e. UserStore, ProductStore, etc
     * which the service layer utilizes. When the Router is created, this
     * registry is injected into the ServiceRegistry.
     */
    std::unique_ptr<StoreRegistry> m_storeRegistry;
};