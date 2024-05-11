#include "store_registry.hpp"

std::shared_ptr<UserStore> StoreRegistry::getUserStore() {
    return m_userStore;
}
