#include "user_model.hpp"
#include <random>
#include <utility>

UserModel::UserModel(
std::string firstName,
std::string lastName
) :
m_firstName(std::move(firstName)),
m_lastName(std::move(lastName))
{
    m_id = randomId();
    m_created = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
    m_modified = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count();
}

uint64_t UserModel::getId() const {
    return m_id;
}

uint64_t UserModel::randomId() {
    using engine = std::default_random_engine;
    using dist = std::uniform_int_distribution<>;

    std::random_device r;
    engine e{r()};
    dist d{1000, 9999};
    return d(e);
}
