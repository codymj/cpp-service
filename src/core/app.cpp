#include "app.hpp"
#include "handler_factory.hpp"

int App::main(const std::vector<std::string>&) {
    auto port = (unsigned short) config().getInt("HttpServer.port", 8080);

    ServerSocket ss(port);

    HTTPServer server(new HandlerFactory, ss, new HTTPServerParams);
    server.start();

    waitForTerminationRequest();
    server.stop();

    return Application::EXIT_OK;
}
