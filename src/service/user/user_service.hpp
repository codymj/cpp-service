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

    explicit UserService(std::shared_ptr<UserStore> store)
    : m_userStore(std::move(store))
    {}

    /**
     * Gets users from the data store and returns results to the handler.
     * @return Collection of users.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

private:
    std::shared_ptr<UserStore> m_userStore;
};
