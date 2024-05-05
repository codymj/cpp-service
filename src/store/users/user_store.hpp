#pragma once

#include "user_model.hpp"
#include <memory>
#include <vector>

class UserStore {
public:
    std::vector<UserModel> getUsers();

private:
    std::vector<UserModel> m_users;
};
