#pragma once

#include "../../store/user/postgres_user_store.hpp"
#include <memory>
#include <utility>
#include <vector>

/**
 * Service for manipulating User businless logic.
 */
class UserService
{
public:
    /**
     * Don't want to lazily create or copy/move this.
     */
    UserService() = delete;
    UserService(UserService&) = delete;
    UserService(UserService&&) = delete;

    explicit UserService(PostgresUserStore* userStore)
    : m_userStore(userStore)
    {}

    /**
     * Gets users from the data store and returns results to the handler.
     * @return Collection of users.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

    void
    saveUser(User const& user);

private:
    PostgresUserStore* m_userStore;
};
