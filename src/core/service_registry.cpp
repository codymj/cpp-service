#include "service_registry.hpp"

user_service* service_registry::get_user_service() const
{
    return m_user_service.get();
}
