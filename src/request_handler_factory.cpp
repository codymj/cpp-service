#include "request_handler_factory.hpp"

HTTPRequestHandler* RequestHandlerFactory::hello_handler() {
    return new HelloRequestHandler;
}
