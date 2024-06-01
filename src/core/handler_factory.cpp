#include "handler_factory.hpp"
#include <Poco/Net/HTTPServerRequest.h>

HTTPRequestHandler* HandlerFactory::createRequestHandler
(
    HTTPServerRequest const& req
)
{
    RouteKey const rk{req.getMethod(), req.getURI()};
    return m_router->lookupHandler(rk);
}
