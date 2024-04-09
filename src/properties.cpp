#include "properties.hpp"

void Properties::Loader::load(std::string json_file) {
	m_log = quill::create_logger("properties");

	// load configuration settings from file
	parser json_parser;
	const auto json = padded_string::load(json_file);
	if (json.error()) {
		LOG_CRITICAL(m_log, "Error loading configuration file: {}", json_file);
		throw std::invalid_argument(simdjson::error_message(json.error()));
	}
	document doc = json_parser.iterate(json);

	load_application_props(doc);
	load_server_props(doc);
	load_database_props(doc);
}

void Properties::Loader::load_application_props(document& doc) {
	std::string_view tmpStr{};

	simdjson::error_code err = doc["application"]["domain"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid application.domain");
	}
	application.domain = std::string(tmpStr);

	err = doc["application"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid application.name");
	}
	application.name = std::string(tmpStr);

	// todo: should have format <int>.<int>.<int>
	err = doc["application"]["version"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid application.version");
	}
	application.version = std::string(tmpStr);
}

void Properties::Loader::load_server_props(document& doc) {
	uint64_t tmpInt{};

	simdjson::error_code err = doc["server"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.port");
	}
	server.port = static_cast<uint16_t>(tmpInt);

	err = doc["server"]["read_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.read_timeout");
	}
	server.read_timeout = static_cast<uint8_t>(tmpInt);

	err = doc["server"]["write_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.write_timeout");
	}
	server.write_timeout = static_cast<uint8_t>(tmpInt);

	err = doc["server"]["idle_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid server.idle_timeout");
	}
	server.idle_timeout = static_cast<uint8_t>(tmpInt);
}

void Properties::Loader::load_database_props(document &doc) {
	std::string_view tmpStr{};
	uint64_t tmpInt{};

	simdjson::error_code err = doc["database"]["host"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.host");
	}
	database.host = std::string(tmpStr);

	err = doc["database"]["port"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.port");
	}
	database.port = static_cast<uint16_t>(tmpInt);

	err = doc["database"]["username"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.username");
	}
	database.username = std::string(tmpStr);

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
	database.password = password;

	err = doc["database"]["name"].get(tmpStr);
	if (err) {
		throw std::invalid_argument("Invalid database.name");
	}
	database.name = std::string(tmpStr);

	err = doc["database"]["connection_timeout"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.connection_timeout");
	}
	database.connection_timeout = static_cast<uint8_t>(tmpInt);

	err = doc["database"]["max_open_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_open_connections");
	}
	database.max_open_connections = static_cast<uint8_t>(tmpInt);

	err = doc["database"]["max_idle_connections"].get(tmpInt);
	if (err) {
		throw std::invalid_argument("Invalid database.max_idle_connections");
	}
	database.max_idle_connections = static_cast<uint8_t>(tmpInt);
}
