#include "user_service.hpp"

std::unique_ptr<std::vector<user>> user_service::get_users() const
{
    //return m_user_store->get_users();

    // Mock response.
    user u0(
        1,
        "codyj@protonmail.com",
        "somepass123",
        "Cody",
        "Johnson",
        1737863161,
        1737863161
    );

    auto users = std::make_unique<std::vector<user>>();
    users->push_back(u0);
    return users;
}

void user_service::save_user(user const& user) const
{
    m_user_store->save_user(user);
}
