#include "store_registry.hpp"

PostgresUserStore* StoreRegistry::getUserStore()
{
    return m_userStore.get();
}
