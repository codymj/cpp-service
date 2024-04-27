#include "hello.hpp"

void HelloRequestHandler::handleRequest(HTTPServerRequest &req, HTTPServerResponse &res) {
    if (req.getMethod() == "GET") {
        handleGet(req, res);
    }
}

void HelloRequestHandler::handleGet(HTTPServerRequest &req, HTTPServerResponse &res) {
    Application& app = Application::instance();
    app.logger().information("REQ: " + req.getMethod() + " " + req.getURI());

    res.setChunkedTransferEncoding(true);
    res.setContentType("text/plain");

    std::ostream& os = res.send();
    os << "world!";
}
