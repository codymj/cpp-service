#pragma once

#include <chrono>
#include <cstdint>
#include <string>

using std::chrono::year_month_day;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

class UserModel {
public:
    UserModel() = delete;
    UserModel(std::string firstName, std::string lastName);

    [[nodiscard]] uint64_t getId() const;

private:
    static uint64_t randomId();

    uint64_t m_id;
    std::string m_firstName;
    std::string m_lastName;
    uint64_t m_created;
    uint64_t m_modified;
};
