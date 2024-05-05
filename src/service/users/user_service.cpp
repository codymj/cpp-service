#include "user_service.hpp"

std::vector<UserModel> UserService::getUsers() const {
    return m_userStore->getUsers();
}
