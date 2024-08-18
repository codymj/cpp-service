#pragma once

#include <filesystem>
#include <fstream>
#include <yaml-cpp/yaml.h>

/**
 * Class to load configuration settings and environment variables from file.
 */
class config_manager
{
public:
    /**
     * Initialize configuration properties.
     * @param path Configuration file path.
     */
    explicit config_manager(std::filesystem::path const& path)
    {
        load_configuration(path);
    }

    // app ---------------------------------------------------------------------

    [[nodiscard]] std::string const& app_domain() const
    {
        return m_app_domain;
    }

    [[nodiscard]] std::string const& app_name() const
    {
        return m_app_name;
    }

    [[nodiscard]] std::string const& app_version() const
    {
        return m_app_version;
    }

    [[nodiscard]] std::string const& app_log_level() const
    {
        return m_app_log_level;
    }

    // server ------------------------------------------------------------------

    [[nodiscard]] std::string const& server_host() const
    {
        return m_server_host;
    }

    [[nodiscard]] uint16_t server_port() const
    {
        return m_server_port;
    }

    [[nodiscard]] uint16_t server_threads() const
    {
        return m_server_threads;
    }

    [[nodiscard]] uint16_t server_read_timeout() const
    {
        return m_server_read_timeout;
    }

    [[nodiscard]] uint16_t server_write_timeout() const
    {
        return m_server_write_timeout;
    }

    [[nodiscard]] uint16_t server_idle_timeout() const
    {
        return m_server_idle_timeout;
    }

    // database  ---------------------------------------------------------------

    [[nodiscard]] std::string const& database_host() const
    {
        return m_database_host;
    }

    [[nodiscard]] uint16_t database_port() const
    {
        return m_database_port;
    }

    [[nodiscard]] std::string const& database_username() const
    {
        return m_database_username;
    }

    [[nodiscard]] std::string const& database_password() const
    {
        return m_database_password;
    }

    [[nodiscard]] std::string const& database_name() const
    {
        return m_database_name;
    }

    [[nodiscard]] uint16_t database_connection_timeout() const
    {
        return m_database_connection_timeout;
    }

    [[nodiscard]] uint16_t database_connection_pool_size() const
    {
        return m_database_connection_pool_size;
    }

private:
    /**
     * Loads the configuration file and sets properties.
     * @param path Path to properties.yml.
     */
    void load_configuration(std::filesystem::path const& path)
    {
        // Load configuration file.
        YAML::Node cfg = YAML::LoadFile(path);

        // Load environment variables.
        auto const db_pass = cfg["database"]["password"].as<std::string>();
        if (char const* db_pass_env = std::getenv(db_pass.c_str()))
        {
            m_database_password = db_pass_env;
        }

        // Convert properites.
        m_app_domain = cfg["app"]["domain"].as<std::string>();
        m_app_name = cfg["app"]["name"].as<std::string>();
        m_app_version = cfg["app"]["version"].as<std::string>();
        m_app_log_level = cfg["app"]["log_level"].as<std::string>();

        m_server_host = cfg["server"]["host"].as<std::string>();
        m_server_port = cfg["server"]["port"].as<int>();
        m_server_read_timeout = cfg["server"]["read_timeout"].as<int>();
        m_server_threads = cfg["server"]["threads"].as<int>();
        m_server_write_timeout = cfg["server"]["write_timeout"].as<int>();
        m_server_idle_timeout = cfg["server"]["idle_timeout"].as<int>();

        m_database_host = cfg["database"]["host"].as<std::string>();
        m_database_port = cfg["database"]["port"].as<int>();
        m_database_username = cfg["database"]["username"].as<std::string>();
        m_database_name = cfg["database"]["name"].as<std::string>();
        m_database_connection_timeout =
            cfg["database"]["connection_timeout"].as<int>();
        m_database_connection_pool_size =
            cfg["database"]["connection_pool_size"].as<int>();

    }

    std::string m_app_domain;
    std::string m_app_name;
    std::string m_app_version;
    std::string m_app_log_level;

    std::string m_server_host;
    uint16_t m_server_port{};
    uint16_t m_server_threads{};
    uint16_t m_server_read_timeout{};
    uint16_t m_server_write_timeout{};
    uint16_t m_server_idle_timeout{};

    std::string m_database_host;
    uint16_t m_database_port{};
    std::string m_database_username;
    std::string m_database_password;
    std::string m_database_name;
    uint16_t m_database_connection_timeout{};
    uint16_t m_database_connection_pool_size{};
};
