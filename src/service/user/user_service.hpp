#pragma once

#include "../../store/user/user_store.hpp"
#include <memory>
#include <utility>
#include <vector>

class UserService
{
public:
    UserService() = delete;

    UserService(UserService&) = delete;

    explicit UserService(UserStore* userStore)
    : m_userStore(userStore)
    {}

    /**
     * Gets users from the data store and returns results to the handler.
     * @return Collection of users.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

private:
    UserStore* m_userStore;
};
