#include "store_registry.hpp"

postgres_user_store* store_registry::get_user_store() const
{
    return m_user_store.get();
}
