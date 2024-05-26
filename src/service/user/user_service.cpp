#include "user_service.hpp"

std::unique_ptr<std::vector<User>>
UserService::getUsers() const
{
    return m_userStore->getUsers();
}

void
UserService::saveUser(const User &user)
{
    m_userStore->saveUser(user);
}
