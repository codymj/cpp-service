#include "user_store.hpp"

std::vector<UserModel> UserStore::getUsers() {
    return std::vector<UserModel>{
        UserModel("John", "Smith"),
        UserModel("Cody", "Johnson"),
        UserModel("Richard", "von Mises")
    };
}
