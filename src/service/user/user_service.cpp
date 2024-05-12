#include "user_service.hpp"

std::unique_ptr<std::vector<User>>
UserService::getUsers() const
{
    return m_userStore->getUsers();
}
