#include "settings.hpp"

void Settings::load(std::string& config_file) {
    // load configuration settings from file
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(config_file);
    simdjson::ondemand::document doc = parser.iterate(json);

    // validate settings
    std::cout << std::string_view(doc["service"]["namespace"]) << std::endl;
}
