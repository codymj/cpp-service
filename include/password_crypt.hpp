#pragma once

#include <argon2.h>
#include <random>
#include <string>
#include <vector>

/**
 * Simple utility class for hashing passwords.
 */
class password_crypt
{
public:
    /**
     * Generates a random salt.
     * @return salt as std::string.
     */
    static std::string generate_salt()
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());
        std::uniform_int_distribution distribution(0, 255);

        unsigned char salt[m_salt_length];
        for (unsigned char& ch : salt) {
            ch = static_cast<char>(distribution(generator));
        }
        return std::string{reinterpret_cast<char*>(salt), m_salt_length};
    }

    /**
     * Encodes and hashes the supplied password.
     * @param password is the password to hash.
     * @return Encoded hash of the provided password.
     * @throws std::runtime_error.
     */
    static std::string hash_password(std::string const& password)
    {
        // Generate random salt.
        std::string const salt = generate_salt();

        // Get encoded hash length.
        size_t const encodedLength = argon2_encodedlen
        (
            m_iterations,
            m_cost,
            m_parallelism,
            salt.size(),
            m_hash_length,
            Argon2_id
        );

        // Create container for the hash.
        std::vector<char> encoded(encodedLength);

        // Hash the password.
        int const result = argon2id_hash_encoded
        (
            m_iterations,
            m_cost,
            m_parallelism,
            password.c_str(),
            password.size(),
            salt.data(),
            salt.size(),
            m_hash_length,
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
     * @throws std::runtime_error.
     */
    static bool verify_password
    (
        std::string const& password,
        std::string const& encodedHash
    )
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
    /**
     * Number of iterations.
     */
    static constexpr uint32_t m_iterations = 5;

    /**
     * Memory usage in kibibytes.
     */
    static constexpr uint32_t m_cost = 1 << 16;

    /**
     * Number of threads.
     */
    static constexpr uint32_t m_parallelism = 4;

    /**
     * Salt length.
     */
    static constexpr uint32_t m_salt_length = 16;

    /**
     * Hash length.
     */
    static constexpr uint32_t m_hash_length = 16;
};
