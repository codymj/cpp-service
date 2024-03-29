#include "properties.hpp"

void Properties::load(const std::string& config_file) {
	// load configuration settings from file
	simdjson::ondemand::parser json_parser;
	const simdjson::padded_string json= simdjson::padded_string::load(config_file);
	simdjson::ondemand::document doc = json_parser.iterate(json);

	// validate properties in file and store in Properties class
	std::string_view tmpStr{};
	int64_t tmpInt{};

	simdjson::error_code err = doc["service"]["namespace"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.namespace");
	}
	service_namespace = std::string(tmpStr);

	err = doc["service"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.name");
	}
	service_name = std::string(tmpStr);

	// todo: should have format <int>.<int>.<int>
	err = doc["service"]["version"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid service.version");
	}
	service_version = std::string(tmpStr);

	err = doc["server"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.port");
	}
	server_port = static_cast<int>(tmpInt);

	err = doc["server"]["read_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.read_timeout");
	}
	server_read_timeout = static_cast<int>(tmpInt);

	err = doc["server"]["write_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.write_timeout");
	}
	server_write_timeout = static_cast<int>(tmpInt);

	err = doc["server"]["idle_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.idle_timeout");
	}
	server_idle_timeout = static_cast<int>(tmpInt);

	err = doc["database"]["host"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.host");
	}
	database_host = std::string(tmpStr);

	err = doc["database"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.port");
	}
	database_port = static_cast<int>(tmpInt);

	err = doc["database"]["user"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.user");
	}
	database_user = std::string(tmpStr);

	// todo: maybe generalize this for all environment variables?
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

	err = doc["database"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.name");
	}
	database_name = std::string(tmpStr);

	err = doc["database"]["connection_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.connection_timeout");
	}
	database_connection_timeout = static_cast<int>(tmpInt);

	err = doc["database"]["max_open_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_open_connections");
	}
	database_max_open_connections = static_cast<int>(tmpInt);

	err = doc["database"]["max_idle_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_idle_connections");
	}
	database_max_idle_connections = static_cast<int>(tmpInt);
}
