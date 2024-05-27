#include "postgres_user_store.hpp"
#include <iostream>

std::unique_ptr<std::vector<User>>
PostgresUserStore::getUsers() const
{
    // Rent a connection from pool.
    auto cxn = m_connectionPool->rentConnection();

    // Open a transaction.
    pqxx::work txn{*cxn, std::string{"txn"}};

    // Query to get all users.
    std::string query
    {
        "select "
        "user_id, "
        "email, "
        "password, "
        "first_name, "
        "last_name, "
        "floor(extract(epoch FROM created_at)*1000000) as created_at, "
        "floor(extract(epoch FROM modified_at)*1000000) as modified_at "
        "from users"
    };

    // Execute query and build users container.
    auto users = std::make_unique<std::vector<User>>();
    pqxx::result res{};
    try
    {
        res = txn.exec(query);
    }
    catch (pqxx::sql_error const& e)
    {
        // TODO: logging
        std::cerr << e.what() << '\n';
        return users;
    }

    for (auto&& row : res)
    {
        User u{};
        try
        {
            u = {
                row[0].as<uint64_t>(),
                row[1].as<std::string>(),
                row[2].as<std::string>(),
                row[3].as<std::string>(),
                row[4].as<std::string>(),
                row[5].as<uint64_t>(),
                row[6].as<uint64_t>()
            };
        }
        catch (std::exception& e)
        {
            // TODO: logging
            std::cerr << e.what() << '\n';
            return nullptr;
        }

        users->push_back(u);
    }

    // Free connection (IMPORTANT!)
    m_connectionPool->freeConnection(std::move(cxn));

    return users;
}

void
PostgresUserStore::saveUser(const User &user)
{
    // Rent a connection from pool.
    auto cxn = m_connectionPool->rentConnection();

    // Open a transaction.
    pqxx::work txn{*cxn, std::string{"txn"}};

    // Command to save a user.
    std::string command
    {
        "insert into "
        "users (email, password, first_name, last_name) "
        "values ("
        "'" + user.getEmail() + "', "
        "'" + user.getPassword() + "', "
        "'" + user.getFirstName() + "', "
        "'" + user.getLastName() + "')"
    };

    // Execute and commit transaction.
    try
    {
        txn.exec(command);
        txn.commit();
    }
    catch (pqxx::sql_error& e)
    {
        // TODO: logging
        std::cerr << e.what() << '\n' << "Query: " << command << '\n';
    }
    catch (std::exception& e)
    {
        // TODO: logging
        std::cerr << e.what() << '\n' << "Query: " << command << '\n';
    }

    // Free connection (IMPORTANT!)
    m_connectionPool->freeConnection(std::move(cxn));
}
