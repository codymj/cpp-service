#include "store_registry.hpp"

PostgresUserStore* StoreRegistry::getUserStore() const
{
    return m_userStore.get();
}
