#include "store_registry.hpp"

UserStore*
StoreRegistry::getUserStore()
{
    return m_userStore.get();
}
