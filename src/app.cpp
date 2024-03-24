#include <quill/Quill.h>
#include "settings.hpp"

class Settings;

int main(/*int argc, char *argv[]*/) {
	// init logger
	quill::Config quill_cfg;
	quill_cfg.enable_console_colours = true;
	quill::configure(quill_cfg);
	quill::start();
	quill::Logger* logger = quill::get_logger();
	logger->set_log_level(quill::LogLevel::Info);

	// load application settings
	// todo: pass settings file as application arg
	std::string app_cfg_file = "../../settings.json";
	try {
		Settings::load(app_cfg_file);
	} catch (const std::exception& e) {
		LOG_CRITICAL(logger, "Error loading settings: {}", e.what());
		exit(1);
	}

	return 0;
}
