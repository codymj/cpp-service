#pragma once

#include "store_registry.hpp"
#include "../service/user/user_service.hpp"

/**
 * Registry that contains services for business logic and is injected into the
 * Router.
 */
class ServiceRegistry
{
public:
    ServiceRegistry() = delete;

    ServiceRegistry(ServiceRegistry&) = delete;

    ServiceRegistry(ServiceRegistry&&) = delete;

    explicit ServiceRegistry(std::unique_ptr<StoreRegistry> storeRegistry)
    : m_storeRegistry(std::move(storeRegistry))
    {
        // Services get created here.
        m_userService = std::make_unique<UserService>
        (
            m_storeRegistry->getUserStore()
        );
    };

    /**
     * Returns a pointer to the UserService.
     * @return Pointer to the UserService.
     */
    UserService*
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
    std::unique_ptr<UserService> m_userService;
};
