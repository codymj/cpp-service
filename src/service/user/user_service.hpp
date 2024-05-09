#pragma once

#include "../../store/user/user_store.hpp"
#include <memory>
#include <utility>
#include <vector>

class UserService {
public:
    explicit UserService(std::shared_ptr<UserStore> store) :
    m_userStore(std::move(store))
    {}

    [[nodiscard]] std::vector<User> getUsers() const;

private:
    std::shared_ptr<UserStore> m_userStore;
};
