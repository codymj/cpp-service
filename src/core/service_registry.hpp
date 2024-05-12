#pragma once

#include "store_registry.hpp"
#include "../service/user/user_service.hpp"

class ServiceRegistry
{
public:
    ServiceRegistry() = delete;

    ServiceRegistry(ServiceRegistry&) = delete;

    explicit ServiceRegistry(std::unique_ptr<StoreRegistry> storeRegistry)
    : m_storeRegistry(std::move(storeRegistry))
    {
        // Service registries get created here.
        m_userService = std::make_shared<UserService>
        (
            m_storeRegistry->getUserStore()
        );
    };

    /**
     * Returns a shared pointer to the UserService.
     * @return Shared pointer to the UserService.
     */
    std::shared_ptr<UserService>
    getUserService();

private:
    /**
     * Maintains a record of data stores, i.e. UserStore, ProductStore, etc
     * which the service layer utilizes. When the Router is created, this
     * registry is injected into the ServiceRegistry.
     */
    std::unique_ptr<StoreRegistry> m_storeRegistry;

    /**
     * Service layer for User logic.
     */
    std::shared_ptr<UserService> m_userService;
};
