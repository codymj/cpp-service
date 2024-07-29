#pragma once

#include "../../store/user/postgres_user_store.hpp"
#include <memory>
#include <vector>

/**
 * Service for manipulating User businless logic.
 */
class user_service
{
public:
    /**
     * Creates a service for handling User business logic.
     * @param user_store interface for the storage layer.
     */
    explicit user_service(postgres_user_store* user_store)
    : m_user_store(user_store)
    {}

    /**
     * Gets users from the data store and returns results to the handler.
     * @return Collection of users.
     */
    [[nodiscard]] std::unique_ptr<std::vector<user>> get_users() const;

    /**
     * Saves user to the database.
     * @param user User to store in database.
     */
    void save_user(user const& user) const;

private:
    postgres_user_store* m_user_store;
};
