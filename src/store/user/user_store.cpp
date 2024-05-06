#include "user_store.hpp"
#include "../util.hpp"

UserStore::UserStore() {
    Poco::Data::PostgreSQL::Connector::registerConnector();
    std::string cs{"host=localhost port=5432 user=postgres password=sup3rs3cr3tp4ssw0rd dbname=postgres"};
    try {
        Session session(SessionFactory::instance().create("PostgreSQL", cs));
        session.close();
    } catch (Poco::Data::ConnectionFailedException& cfe) {
        std::cout << cfe.what() << '\n';
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }

//    m_users = {
//        User{
//            randomId(),
//            "bsimpson@gmail.com",
//            "h4sh3dp4ssw0rd",
//            "Bart",
//            "Simpson",
//            tsNowMillis(),
//            tsNowMillis()
//        },
//        User{
//            randomId(),
//            "jdoe@gmail.com",
//            "mydogsname99",
//            "Jane",
//            "Doe",
//            tsNowMillis(),
//            tsNowMillis()
//        },
//        User{
//            randomId(),
//            "rvonmises@gmail.com",
//            "passw0rd123",
//            "Richard",
//            "von Mises",
//            tsNowMillis(),
//            tsNowMillis()
//        }
//    };
}

std::vector<User> UserStore::getUsers() {
    return m_users;
}
