#pragma once

#include <chrono>
#include <cstdint>
#include <string>

struct User {
    uint64_t userId;
    std::string email;
    std::string password;
    std::string firstName;
    std::string lastName;
    uint64_t createdAt;
    uint64_t modifiedAt;
};
