#pragma once

#include <memory>
#include <quill/Quill.h>
#include <simdjson.h>

using simdjson::ondemand::document;
using simdjson::ondemand::parser;
using simdjson::padded_string;

namespace Properties {
struct Application {
	std::string domain;
	std::string name;
	std::string version;
};
struct Server {
	uint16_t port;
	uint8_t read_timeout;
	uint8_t write_timeout;
	uint8_t idle_timeout;
};
struct Database {
	std::string host;
	uint16_t port;
	std::string username;
	std::string password;
	std::string name;
	uint8_t connection_timeout;
	uint8_t max_open_connections;
	uint8_t max_idle_connections;
};

class Loader {
public:
	void load(std::string json_file);

	/**
	 * Used to store application-related configuration parameters.
	 */
	Application application{};

	/**
	 * Used to store server-related configuration parameters.
	 */
	Server server{};

	/**
	 * Used to store database-related configuration parameters.
	 */
	Database database{};

private:
	quill::Logger* m_log;

	void load_application_props(document& doc);
	void load_server_props(document& doc);
	void load_database_props(document& doc);
};
}
