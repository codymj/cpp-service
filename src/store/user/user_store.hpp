#pragma once

#include "user_model.hpp"
#include "../db/connection_pool.hpp"
#include <utility>
#include <vector>

using cppservice::database::ConnectionPool;

class UserStore {
public:
    explicit UserStore(std::shared_ptr<ConnectionPool> connectionPool)
    : m_connectionPool(std::move(connectionPool)) {}

    [[nodiscard]] std::unique_ptr<std::vector<User>> getUsers() const;

private:
    std::shared_ptr<ConnectionPool> m_connectionPool;
};
