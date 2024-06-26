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
    /**
     * Initializer for the service registry.
     * @param storeRegistry Store registry for injecting stores into services.
     */
    explicit ServiceRegistry(StoreRegistry* storeRegistry)
    : m_storeRegistry(storeRegistry)
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
    [[nodiscard]] UserService* getUserService() const;

private:
    /**
     * Maintains a record of data stores, i.e. UserStore, ProductStore, etc
     * which the service layer utilizes. When the Router is created, this
     * registry is injected into the ServiceRegistry.
     */
    StoreRegistry* m_storeRegistry{};

    /**
     * Service layer for User logic.
     */
    std::unique_ptr<UserService> m_userService;
};
