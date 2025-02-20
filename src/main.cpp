#include "core/app.hpp"
#include <iostream>

int main(int const argc, char* argv[])
try
{
    if (argc < 2) {
        std::cerr << "Usage: "
            << argv[0]
            << " <path_to_properties.yml>"
            << '\n';
        return 1;
    }

    std::filesystem::path const prop_yml_path(argv[1]);
    if (!exists(prop_yml_path))
    {
        std::cerr << "properties.yml not found: " << prop_yml_path << '\n';
    }

    app app;
    app.initialize(prop_yml_path);
    return app.main();
}
catch (std::filesystem::filesystem_error const& e)
{
    std::cerr << "Filesystem error: " << e.what() << '\n';
    return 2; // Non-zero status for filesystem errors.
}
catch (std::exception const& e)
{
    std::cerr << "General error: " << e.what() << '\n';
    return 3; // Non-zero status for other errors.
}
