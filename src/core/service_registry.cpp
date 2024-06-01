#include "service_registry.hpp"

UserService* ServiceRegistry::getUserService()
{
    return m_userService.get();
}
