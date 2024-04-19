#include <quill/Quill.h>
#include <string>
#include "database.hpp"
#include "properties.hpp"

int main(/*int argc, char *argv[]*/) {
	// init logger
	quill::start();
	quill::Logger* log = quill::create_logger("app");
	LOG_INFO(log, "Starting up...");

	// load application settings
	// todo: pass settings file as application arg
	std::string app_cfg_file = "../../properties.json";

	Properties::Loader props;
	try {
		props.load(app_cfg_file);
	} catch (std::invalid_argument& e) {
		LOG_ERROR(log, "Error loading settings: {}", e.what());
		exit(1);
	} catch (std::exception& e) {
		LOG_ERROR(log, "Internal error: {}", e.what());
		exit(1);
	}
	LOG_INFO(log, "Loaded app configuration from {}", app_cfg_file);

	// init database
	Database db(props.database);
	db.connect();
	LOG_INFO(log, "Connected to database at {}:{}",
		props.database.host,
		props.database.port);

	LOG_INFO(log, "Shutting down...");
	return EXIT_SUCCESS;
}
