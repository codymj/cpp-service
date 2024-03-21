#pragma once

#include <string>
#include "simdjson.h"

class Settings {
public:
    Settings() = delete;

    static void load(std::string& config_file);

    std::string service_namespace;
    std::string service_name;
    std::string service_version;

    int server_port;
    int server_read_timeout;
    int server_write_timeout;
    int server_idle_timeout;

    std::string database_host;
    int database_port;
    std::string database_user;
    std::string database_password;
    std::string database_name;
    int database_max_lifetime;
    int database_max_open_connections;
    int database_max_idle_connections;
};
