#pragma once

#include <string>
#include <utility>

/*
 * Model for users table defined as:
 * users (
 *     user_id bigint primary key generated by default as identity,
 *     email varchar(64) not null,
 *     password varchar(64) not null,
 *     first_name varchar(32) not null,
 *     last_name varchar(32) not null,
 *     created_at timestamp without time zone default (now() at time zone 'utc'),
 *     modified_at timestamp without time zone default (now() at time zone 'utc')
 * );
 */
class User {
public:
    User(
        uint64_t userId,
        std::string email,
        std::string password,
        std::string firstName,
        std::string lastName,
        uint64_t createdAt,
        uint64_t modifiedAt
    )
    : m_email(std::move(email))
    , m_password(std::move(password))
    , m_firstName(std::move(firstName))
    , m_lastName(std::move(lastName))
    , m_userId(userId)
    , m_createdAt(createdAt)
    , m_modifiedAt(modifiedAt) {}

    [[nodiscard]] uint64_t getUserId() const {
        return m_userId;
    }
    void setUserId(uint64_t uid) {
        m_userId = uid;
    }

    [[nodiscard]] std::string getEmail() const {
        return m_email;
    }
    void setEmail(std::string email) {
        m_email = std::move(email);
    }

    [[nodiscard]] std::string getPassword() const {
        return m_password;
    }
    void setPassword(std::string password) {
        m_password = std::move(password);
    }

    [[nodiscard]] std::string getFirstName() const {
        return m_firstName;
    }
    void setFirstName(std::string firstName) {
        m_firstName = std::move(firstName);
    }

    [[nodiscard]] std::string getLastName() const {
        return m_lastName;
    }
    void setLastName(std::string lastName) {
        m_lastName = std::move(lastName);
    }

    [[nodiscard]] uint64_t getCreatedAt() const {
        return m_createdAt;
    }
    void setCreatedAt(uint64_t createdAt) {
        m_createdAt = createdAt;
    }

    [[nodiscard]] uint64_t getModifiedAt() const {
        return m_modifiedAt;
    }
    void setModifiedAt(uint64_t modifiedAt) {
        m_modifiedAt = modifiedAt;
    }

private:
    std::string m_email;
    std::string m_password;
    std::string m_firstName;
    std::string m_lastName;
    uint64_t m_userId;
    uint64_t m_createdAt;
    uint64_t m_modifiedAt;
};
