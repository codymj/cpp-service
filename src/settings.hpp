#pragma once

#include <any>
#include <map>
#include <simdjson.h>
#include <string>

class Settings {
public:
	/**
	 * Attemps to validate parameters from a config file and stores them in Settings::properties.
	 * @param config_file JSON file containing properties for the web service.
	 */
	static void load(const std::string& config_file);

	/**
	 * Map containing application settings loaded from a configuration file.
	 */
	inline static std::map<std::string, std::any> properties;

private:
};
