#include "user_store.hpp"
#include "../util.hpp"

std::unique_ptr<std::vector<User>> UserStore::getUsers() const {
    // Get connection from pool.
    auto cxn = m_connectionPool->getConnection();

    // Open a transaction.
    // pqxx::work txn{cxn.get(), std::string{"txn"}};

    // Free connection (IMPORTANT!)
    m_connectionPool->freeConnection(cxn);

    return std::make_unique<std::vector<User>>();
}
