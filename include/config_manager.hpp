#pragma once

#include <mutex>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>

/**
 * Singleton to load configuration settings from file.
 */
class ConfigManager
{
public:
    /**
     * Disable copy.
     */
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    /**
     * Returns the only instance of ConfigManager.
     * @return The only instance of ConfigManager.
     */
    static ConfigManager& instance()
    {
        static ConfigManager instance;
        return instance;
    }

    /**
     * Loads settings from file.
     * @param path File to load settings from.
     */
    void initialize(std::string const& path)
    {
        std::call_once
        (
            m_initFlag,
            &ConfigManager::loadConfiguration,
            this,
            path
        );
    }

    /**
     * Returns configuration properties.
     * @return Property file configuration.
     */
    [[nodiscard]] Poco::Util::AbstractConfiguration const& config() const
    {
        return *m_config;
    }

private:
    /**
     * Disable construction.
     */
    ConfigManager() = default;

    /**
     * Calls Poco's loadConfiguration().
     * @param path Configuration file path.
     */
    explicit ConfigManager(std::string const& path)
    {
        loadConfiguration(path);
    }

    /**
     * Poco loads the configuration file.
     * @param path
     */
    void loadConfiguration(std::string const& path)
    {
        m_config = new Poco::Util::PropertyFileConfiguration(path);
    }

    /**
     * Pointer to configuration properties.
     */
    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> m_config;

    /**
     * Only initialize once.
     */
    std::once_flag m_initFlag;
};
