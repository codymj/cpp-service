#pragma once

#include "../../store/user/user_store.hpp"
#include <memory>
#include <vector>

class UserService {
public:
    UserService() {
        m_userStore = std::make_shared<UserStore>();
    }

    [[nodiscard]] std::vector<User> getUsers() const;

private:
    std::shared_ptr<UserStore> m_userStore;
};
