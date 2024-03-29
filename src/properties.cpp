#include "properties.hpp"

void Properties::load(const std::string& config_file) {
	using simdjson::ondemand::parser;
	using simdjson::ondemand::document;
	using simdjson::padded_string;

	// load configuration settings from file
	parser parser;
	const padded_string json = padded_string::load(config_file);
	document doc = parser.iterate(json);

	// validate properties in file and store in Properties class
	simdjson::error_code err;
	std::string_view tmpStr{};
	int64_t tmpInt{};

	// service.namespace
	err = doc["service"]["namespace"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.namespace");
	}
	service_namespace = std::string(tmpStr);

	// service.name
	err = doc["service"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.name");
	}
	service_name = std::string(tmpStr);

	// service.version
	// todo: should have format <int>.<int>.<int>
	err = doc["service"]["version"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.version");
	}
	service_version = std::string(tmpStr);

	// server.port
	err = doc["server"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.port");
	}
	server_port = static_cast<int>(tmpInt);

	// server.read_timeout
	err = doc["server"]["read_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.read_timeout");
	}
	server_read_timeout = static_cast<int>(tmpInt);

	// server.write_timeout
	err = doc["server"]["write_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.write_timeout");
	}
	server_write_timeout = static_cast<int>(tmpInt);

	// server.idle_timeout
	err = doc["server"]["idle_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.idle_timeout");
	}
	server_idle_timeout = static_cast<int>(tmpInt);

	// database.host
	err = doc["database"]["host"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.host");
	}
	database_host = std::string(tmpStr);

	// database.port
	err = doc["database"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.port");
	}
	database_port = static_cast<int>(tmpInt);

	// database.user
	err = doc["database"]["user"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.user");
	}
	database_user = std::string(tmpStr);

	// database.password
	// todo: maybe generalize this?
	err = doc["database"]["password"].get(tmpStr);
	if (err) {
		std::string msg{
			"Invalid database.password: Use name of env variable."};
		throw std::invalid_argument(msg);
	}
	const std::string tmp{tmpStr.begin(), tmpStr.end()};
	const char* password = std::getenv(tmp.c_str());
	if (password == nullptr) {
		std::string msg{
			"Make sure $POSTGRES_PASS env variable has been set."
		};
		throw std::invalid_argument(msg);
	}
	database_password = password;

	// database.name
	err = doc["database"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.name");
	}
	database_name = std::string(tmpStr);

	// database.connection_max_lifetime
	err = doc["database"]["connection_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.connection_timeout");
	}
	database_connection_timeout = static_cast<int>(tmpInt);

	// database.max_open_connections
	err = doc["database"]["max_open_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_open_connections");
	}
	database_max_open_connections = static_cast<int>(tmpInt);

	// database.max_idle_connections
	err = doc["database"]["max_idle_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_idle_connections");
	}
	database_max_idle_connections = static_cast<int>(tmpInt);
}
