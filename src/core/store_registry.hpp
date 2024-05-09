#pragma once

#include "../store/user/user_store.hpp"
#include <memory>

class StoreRegistry {
public:
    StoreRegistry();

    std::shared_ptr<UserStore> getUserStore();

private:
    /**
     * Data store for User logic.
     */
    std::shared_ptr<UserStore> m_userStore;
};
