#include "user_service.hpp"

std::vector<User> UserService::getUsers() const {
    return m_userStore->getUsers();
}
