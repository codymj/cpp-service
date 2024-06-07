#pragma once

#include <argon2.h>
#include <random>
#include <string>
#include <vector>

/**
 * Simple utility class for hashing passwords.
 */
class PasswordCrypt
{
public:
    /**
     * Generates a random salt.
     * @return salt as std::string.
     */
    static std::string generateSalt()
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_int_distribution distribution(0, 255);

        unsigned char salt[saltLength];
        for (unsigned char& ch : salt) {
            ch = static_cast<char>(distribution(generator));
        }
        return std::string{reinterpret_cast<char*>(salt), saltLength};
    }

    /**
     * Encodes and hashes the supplied password.
     * @param password is the password to hash.
     * @return Encoded hash of the provided password.
     */
    static std::string hashPassword(std::string const& password)
    {
        // Generate random salt.
        std::string const salt = generateSalt();

        // Get encoded hash length.
        size_t const encodedLength = argon2_encodedlen
        (
            t_cost,
            m_cost,
            parallelism,
            salt.size(),
            hashLength,
            Argon2_id
        );

        // Create container for the hash.
        std::vector<char> encoded(encodedLength);

        // Hash the password.
        int const result = argon2id_hash_encoded
        (
            t_cost,
            m_cost,
            parallelism,
            password.c_str(),
            password.size(),
            salt.data(),
            salt.size(),
            hashLength,
            encoded.data(),
            encoded.size()
        );
        if (result != ARGON2_OK)
        {
            throw std::runtime_error(argon2_error_message(result));
        }

        return encoded.data();
    }

    /**
     * Verifies that the provided encoded hash matches the provided password.
     * @param password is the provided password.
     * @param encodedHash the encoded hash of the password to verify.
     * @return True on match, false otherwise.
     */
    static bool
    verifyPassword(std::string const& password, std::string const& encodedHash)
    {
        int const result = argon2i_verify
        (
            encodedHash.c_str(),
            password.c_str(),
            password.size()
        );
        if (result == ARGON2_OK)
        {
            return true;
        }
        if (result == ARGON2_VERIFY_MISMATCH)
        {
            return false;
        }

        throw std::runtime_error(argon2_error_message(result));
    }

private:
    static constexpr uint32_t t_cost = 5;       // Number of iterations
    static constexpr uint32_t m_cost = 1 << 16; // Memory usage in kibibytes
    static constexpr uint32_t parallelism = 4;  // Number of threads
    static constexpr uint32_t saltLength = 16;  // Salt length
    static constexpr uint32_t hashLength = 16;  // Hash length
};
