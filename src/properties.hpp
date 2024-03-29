#pragma once

#include <simdjson.h>

class Properties {
public:
	/**
	 * Attemps to validate parameters from a config file and stores them in
	 * Settings::properties.
	 * @param config_file JSON file containing properties for the web service.
	 */
	static void load(const std::string& config_file);

	/**
	 * service_namespace is for grouping similar services by domain.
	 */
	inline static std::string service_namespace;

	/**
	 * service_name is the name of the service.
	 */
	inline static std::string service_name;

	/**
	 * service_version is used for versioning: [major].[minor].[patch], i.e.
	 * 1.0.3
	 */
	inline static std::string service_version;

	/**
	 * server_port is the port the service is accepting requests from.
	 */
	inline static int server_port;

	/**
	 * server_write_timeout is the maximum time (in seconds) to send a response.
	 */
	inline static int server_write_timeout;

	/**
	 * server_read_timeout is the maximum time (in seconds) to wait for a
	 * request.
	 */
	inline static int server_read_timeout;

	/**
	 * server_idle_timeout is the maximum time (in seconds) a connection can
	 * remain inactive before it is closed.
	 */
	inline static int server_idle_timeout;

	/**
	 * database_host is the IP where the database is hosted.
	 */
	inline static std::string database_host;

	/**
	 * database_port is the port the database is accepting requests from.
	 */
	inline static int database_port;

	/**
	 * database_user is the username required to connect to the database.
	 */
	inline static std::string database_user;

	/**
	 * database_password is the password required to connect to the database.
	 */
	inline static std::string database_password;

	/**
	 * database_name is the name of the database to connect to.
	 */
	inline static std::string database_name;

	/**
	 * database_connection_timeout is the maximum time (in seconds) before an
	 * inactive connection is closed.
	 */
	inline static int database_connection_timeout;

	/**
	 * database_max_open_connections is the maximum amount of active connections
	 * to the database.
	 */
	inline static int database_max_open_connections;

	/**
	 * database_max_idle_connections is the maximum amount of inactive
	 * connections to the database.
	 */
	inline static int database_max_idle_connections;
};
