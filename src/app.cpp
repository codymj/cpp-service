#include "app.hpp"
#include "request_handler_factory.hpp"

[[maybe_unused]] void App::initialize(Poco::Util::Application &self) {
    loadConfiguration();
    ServerApplication::initialize(self);
}

[[maybe_unused]] void App::uninitialize() {
    ServerApplication::uninitialize();
}

int App::main(const std::vector<std::string>&) {
    auto port = (unsigned short) config().getInt("HttpServer.port", 8080);

    ServerSocket ss(port);

    HTTPServer server(new RequestHandlerFactory, ss, new HTTPServerParams);
    server.start();

    waitForTerminationRequest();
    server.stop();

    return Application::EXIT_OK;
}
