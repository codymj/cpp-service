#include "store_registry.hpp"

StoreRegistry::StoreRegistry() {
    m_userStore = std::make_shared<UserStore>();
}

std::shared_ptr<UserStore> StoreRegistry::getUserStore() {
    return m_userStore;
}
