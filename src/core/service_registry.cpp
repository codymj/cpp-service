#include "service_registry.hpp"

UserService* ServiceRegistry::getUserService() const
{
    return m_userService.get();
}
