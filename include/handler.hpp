#pragma once

#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

/**
 * Base class for HTTP handlers.
 */
class handler
{
public:
    virtual ~handler() = default;

    /**
     * Handles an HTTP request.
     * @param req HTTP request.
     * @param res HTTP response.
     * @return Type-erased HTTP response.
     */
    virtual http::message_generator handle
    (
        http::request<http::string_body> req,
        http::response<http::string_body> res
    ) = 0;
};
