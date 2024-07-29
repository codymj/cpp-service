#include "user_service.hpp"

std::unique_ptr<std::vector<user>> user_service::get_users() const
{
    return m_user_store->get_users();
}

void user_service::save_user(user const& user) const
{
    m_user_store->save_user(user);
}
