#include "handler_factory.hpp"

HandlerFactory::HandlerFactory() {
    m_router = std::make_unique<Router>();
}

HTTPRequestHandler* HandlerFactory::createRequestHandler(HTTPServerRequest const& req) {
    RouteKey const rk{req.getMethod(), req.getURI()};
    return m_router->lookupHandler(rk);
}
