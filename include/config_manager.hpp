#pragma once

#include <mutex>
#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>

class ConfigManager
{
public:
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    static ConfigManager& instance()
    {
        static ConfigManager instance;
        return instance;
    }

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

    [[nodiscard]] Poco::Util::AbstractConfiguration const& config() const
    {
        return *m_config;
    }

private:
    ConfigManager() = default;

    explicit ConfigManager(std::string const& path)
    {
        loadConfiguration(path);
    }

    void loadConfiguration(std::string const& path)
    {
        m_config = new Poco::Util::PropertyFileConfiguration(path);
    }

    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> m_config;
    std::once_flag m_initFlag;
};
