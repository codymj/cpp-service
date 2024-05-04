#include "handler_factory.hpp"

HTTPRequestHandler* HandlerFactory::createRequestHandler(HTTPServerRequest const& req) {
    RouteKey const rk{req.getMethod(), req.getURI()};
    return m_router.lookupHandler(rk);
}
