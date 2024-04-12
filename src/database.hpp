#pragma once

#include <pqxx/pqxx>
#include "properties.hpp"

class Database {
public:
	pqxx::connection connection;

	explicit Database(Properties::Database& db_props) : m_props(db_props) {};

	void connect();

private:
	Properties::Database m_props;
};
