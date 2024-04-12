#include "database.hpp"

void Database::connect() {
	// postgresql://<username>:<password>@<host>:<port>/<db>?<params>
	// todo: set up ssl and other parameters
	std::string cs{"postgresql://" +
		m_props.username + ":" + m_props.password + "@" +
		m_props.host + ":" +
		std::to_string(m_props.port) + "/" +
		m_props.name + "?" +
		"connect_timeout=" + std::to_string(m_props.connection_timeout)};

	connection = pqxx::connection{cs};
}
