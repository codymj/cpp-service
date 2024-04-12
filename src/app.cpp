#include <quill/Quill.h>
#include <string>
#include "database.hpp"
#include "properties.hpp"

int main(/*int argc, char *argv[]*/) {
	// init logger
	quill::Config quill_cfg;
	quill_cfg.enable_console_colours = true;
	quill::configure(quill_cfg);
	quill::start();
	quill::Logger* log = quill::get_logger();
	log->set_log_level(quill::LogLevel::Info);

	// load application settings
	// todo: pass settings file as application arg
	std::string app_cfg_file = "../../properties.json";

	Properties::Loader props;
	try {
		props.load(app_cfg_file);
	} catch (std::invalid_argument& e) {
		LOG_CRITICAL(log, "Error loading settings: {}", e.what());
		exit(1);
	} catch (std::exception& e) {
		LOG_CRITICAL(log, "Internal error: {}", e.what());
		exit(1);
	}

	// init database
	Database db(props.database);
	db.connect();

	return 0;
}
