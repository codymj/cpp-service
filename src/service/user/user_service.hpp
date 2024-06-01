#pragma once

#include "../../store/user/postgres_user_store.hpp"
#include <memory>
#include <vector>

/**
 * Service for manipulating User businless logic.
 */
class UserService
{
public:
    /**
     * Creates a service for handling User business logic.
     * @param userStore interface for the storage layer.
     */
    explicit UserService(PostgresUserStore* userStore)
    : m_userStore(userStore)
    {}

    /**
     * Gets users from the data store and returns results to the handler.
     * @return Collection of users.
     */
    [[nodiscard]] std::unique_ptr<std::vector<User>> getUsers() const;

    /**
     * Saves user to the database.
     * @param user User to store in database.
     */
    void saveUser(User const& user) const;

private:
    PostgresUserStore* m_userStore;
};
