#include "request_handler_factory.hpp"

HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(HTTPServerRequest const& req) {
    if (req.getURI() == "/hello") {
        return hello_handler();
    } else {
        return nullptr;
    }
}

HTTPRequestHandler* RequestHandlerFactory::hello_handler() {
    return new HelloRequestHandler;
}
