#include "postgres_user_store.hpp"
#include <context.hpp>
#include <defer.hpp>
#include <exception>
#include <quill/LogMacros.h>

std::unique_ptr<std::vector<user>> postgres_user_store::get_users() const
{
    // Rent a connection from pool.
    auto cxn = m_connection_pool->rent();
    DEFER(m_connection_pool->free(std::move(cxn)));

    // Open a transaction.
    pqxx::work txn{*cxn, std::string{"txn"}};

    // Query to get all users.
    std::string const query
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
    auto users = std::make_unique<std::vector<user>>();
    pqxx::result res{};
    try
    {
        res = txn.exec(query);
    }
    catch (std::exception const& e)
    {
        LOG_ERROR(m_logger, "{error}", e.what());
        throw;
    }

    // Parse rows in response.
    for (auto&& row : res)
    {
        user u{};
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
        catch (std::exception const& e)
        {
            LOG_ERROR(m_logger, "{error}", e.what());
            throw;
        }

        users->push_back(u);
    }

    return users;
}

void postgres_user_store::save_user(user const& user) const
{
    // Rent a connection from pool.
    auto cxn = m_connection_pool->rent();
    DEFER(m_connection_pool->free(std::move(cxn)));

    // Open a transaction.
    pqxx::work txn{*cxn, std::string{"txn"}};

    // Command to save a user.
    std::string const command
    {
        "insert into "
        "users (email, password, first_name, last_name) "
        "values ("
        "'" + user.get_email() + "', "
        "'" + user.get_password() + "', "
        "'" + user.get_first_name() + "', "
        "'" + user.get_last_name() + "')"
    };

    // Execute and commit transaction.
    try
    {
        txn.exec(command);
        txn.commit();
    }
    catch (std::exception& e)
    {
        LOG_ERROR(m_logger, "{error}", e.what());
        throw;
    }
}
