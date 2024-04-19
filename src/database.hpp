#pragma once

#include <pqxx/pqxx>
#include "properties.hpp"

class Database {
public:
	explicit Database(Properties::Database& db_props) : m_props(db_props) {};
	void connect();

	pqxx::connection connection;

private:
	Properties::Database m_props;
};
