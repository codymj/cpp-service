#include <cstdlib>
#include <stdexcept>
#include "settings.hpp"

void Settings::load(const std::string& config_file) {
	// load configuration settings from file
	simdjson::ondemand::parser parser;
	const simdjson::padded_string json = simdjson::padded_string::load(config_file);
	simdjson::ondemand::document doc = parser.iterate(json);

	try {
		// service.namespace
		if (std::string_view(doc["service"]["namespace"]).empty()) {
			throw std::invalid_argument("service.namespace cannot be empty");
		}
		properties["service_namespace"] = std::string_view(doc["service"]["namespace"]);

		// service.name
		if (std::string_view(doc["service"]["name"]).empty()) {
			throw std::invalid_argument("service.name cannot be empty");
		}
		properties["service_name"] = std::string_view(doc["service"]["name"]);

		// service.version
		// todo: should have format <int>.<int>.<int>
		if (std::string_view(doc["service"]["version"]).empty()) {
			throw std::invalid_argument("service.version cannot be empty");
		}
		properties["service_version"] = std::string_view(doc["service"]["version"]);

		// server.port
		if (static_cast<int64_t>(doc["server"]["port"])<0 || static_cast<int64_t>(doc["server"]["port"])>65535) {
			throw std::invalid_argument("server.port must be between 0 and 65535");
		}
		properties["server_port"] = static_cast<int64_t>(doc["server"]["port"]);

		// server.read_timeout
		if (static_cast<int64_t>(doc["server"]["read_timeout"])<0) {
			throw std::invalid_argument("server.read_timeout must be 0 seconds or more");
		}
		properties["server_read_timeout"] = static_cast<int64_t>(doc["server"]["read_timeout"]);

		// server.write_timeout
		if (static_cast<int64_t>(doc["server"]["write_timeout"])<0) {
			throw std::invalid_argument(
				"server.write_timeout must be 0 seconds or more");
		}
		properties["server_write_timeout"] = static_cast<int64_t>(doc["server"]["write_timeout"]);

		// server.idle_timeout
		if (static_cast<int64_t>(doc["server"]["idle_timeout"])<0) {
			throw std::invalid_argument(
				"server.idle_timeout must be 0 seconds or more");
		}
		properties["server_idle_timeout"] = static_cast<int64_t>(doc["server"]["idle_timeout"]);

		// database.host
		if (std::string_view(doc["database"]["host"]).empty()) {
			throw std::invalid_argument("database.host cannot be empty");
		}
		properties["database_host"] = std::string_view(doc["database"]["host"]);

		// database.port
		if (static_cast<int64_t>(doc["database"]["port"])<0 || static_cast<int64_t>(doc["database"]["port"])>65535) {
			throw std::invalid_argument("database.port must be between 0 and 65535");
		}
		properties["database_port"] = static_cast<int64_t>(doc["database"]["port"]);

		// database.user
		if (std::string_view(doc["database"]["user"]).empty()) {
			throw std::invalid_argument("database.user cannot be empty");
		}
		properties["database_user"] = std::string_view(doc["database"]["user"]);

		// database.password
		auto password_env{std::string_view(doc["database"]["password"])};
		if (std::string_view(doc["database"]["password"]).empty()) {
			throw std::invalid_argument("database.password cannot be empty");
		}
		const std::string tmp{password_env.begin(), password_env.end()};
		const char* password = std::getenv(tmp.c_str());
		if (password == nullptr) {
			throw std::invalid_argument("Please set a database password environment variable");
		}
		properties["database_password"] = std::string_view{password};

		// database.name
		if (std::string_view(doc["database"]["name"]).empty()) {
			throw std::invalid_argument("database.name cannot be empty");
		}
		properties["database_name"] = std::string_view(doc["database"]["name"]);

		// database.connection_max_lifetime
		if (static_cast<int64_t>(doc["database"]["connection_max_lifetime"])<0) {
			throw std::invalid_argument("database.connection_max_lifetime must be 0 or greater");
		}
		properties["database_connection_max_lifetime"] = static_cast<int64_t>(doc["database"]["connection_max_lifetime"]);

		// database.max_open_connections
		if (static_cast<int64_t>(doc["database"]["max_open_connections"])<0) {
			throw std::invalid_argument("database.max_open_connections must be 0 or greater");
		}
		properties["database_max_open_connections"] = static_cast<int64_t>(doc["database"]["max_open_connections"]);

		// database.max_idle_connections
		if (static_cast<int64_t>(doc["database"]["max_idle_connections"])<0) {
			throw std::invalid_argument("database.max_idle_connections must be 0 or greater");
		}
		properties["database_max_idle_connections"] = static_cast<int64_t>(doc["database"]["max_idle_connections"]);

	} catch (const simdjson::simdjson_error&) {
		throw;
	}
}
