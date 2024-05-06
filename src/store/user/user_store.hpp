#pragma once

#include "user_model.hpp"
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/PostgreSQL/Connector.h>
#include <vector>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::SessionFactory;
using Poco::Data::Statement;

class UserStore {
public:
    UserStore();

    std::vector<User> getUsers();

private:
    std::vector<User> m_users;
};
