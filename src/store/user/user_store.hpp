#pragma once

#include "user_model.hpp"
#include <connection_pool.hpp>
#include <utility>
#include <vector>

using cppservice::database::ConnectionPool;

class UserStore
{
public:
    UserStore() = delete;

    UserStore(UserStore&) = delete;

    explicit UserStore(std::shared_ptr<ConnectionPool> connectionPool)
    : m_connectionPool(std::move(connectionPool))
    {}

    /**
     * Gets all users from the database.
     * TODO: Implement query parameters.
     * @return All users in database.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

private:
    std::shared_ptr<ConnectionPool> m_connectionPool;
};
