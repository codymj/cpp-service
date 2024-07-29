#pragma once

#include "store_registry.hpp"
#include "../service/user/user_service.hpp"

/**
 * Contains services for business logic and is injected into the router.
 */
class service_registry
{
public:
    /**
     * Initializer for the service registry.
     * @param store_registry Store registry for injecting stores into services.
     */
    explicit service_registry(store_registry* store_registry)
    : m_store_registry(store_registry)
    {
        // Services get created here.
        m_user_service = std::make_unique<user_service>
        (
            m_store_registry->get_user_store()
        );
    };

    /**
     * Returns a pointer to the UserService.
     * @return Pointer to the UserService.
     */
    [[nodiscard]] user_service* get_user_service() const;

private:
    /**
     * Maintains a record of data stores, i.e. UserStore, ProductStore, etc
     * which the service layer utilizes. When the Router is created, this
     * registry is injected into the ServiceRegistry.
     */
    store_registry* m_store_registry{};

    /**
     * Service layer for User logic.
     */
    std::unique_ptr<user_service> m_user_service;
};
