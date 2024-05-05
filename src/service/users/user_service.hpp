#pragma once

#include "../../store/users/user_store.hpp"
#include <memory>
#include <vector>

class UserService {
public:
    UserService() {
        m_userStore = std::make_shared<UserStore>();
    }

    [[nodiscard]] std::vector<UserModel> getUsers() const;

private:
    std::shared_ptr<UserStore> m_userStore;
};
