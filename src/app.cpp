#include "app.hpp"

int main(/*int argc, char *argv[]*/) {
    std::string cfg = "../../settings.json";
    Settings::load(cfg);

    return 0;
}
