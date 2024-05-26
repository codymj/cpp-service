#pragma once

#include "user_model.hpp"
#include "postgres_connection.hpp"
#include <utility>
#include <vector>

using cppservice::database::ConnectionPool;

class UserStore
{
public:
    UserStore() = delete;

    UserStore(UserStore&) = delete;

    UserStore(UserStore&&) = delete;

    explicit UserStore
    (
        ConnectionPool<PqxxPtr>* connectionPool
    )
    : m_connectionPool(connectionPool)
    {}

    /**
     * Gets all users from the database.
     * TODO: Implement query parameters.
     * @return All users in database.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>>
    getUsers() const;

private:
    ConnectionPool<PqxxPtr>* m_connectionPool;
};
