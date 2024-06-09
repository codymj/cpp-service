#include "handler_factory.hpp"

#include <context.hpp>
#include <Poco/Net/HTTPServerRequest.h>

HTTPRequestHandler* HandlerFactory::createRequestHandler
(
    HTTPServerRequest const& req
)
{
    // Initialize context for this request.
    auto const ctx = std::make_shared<Context>();
    Context::setContext(ctx);

    // Find handler in router.
    return m_router->lookupHandler({req.getMethod(), req.getURI()});
}
