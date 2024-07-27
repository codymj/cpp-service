#pragma once

#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

class handler
{
public:
    virtual ~handler() = default;

    virtual http::message_generator handle
    (
        http::request<http::string_body> req,
        http::response<http::string_body> res
    ) = 0;
};
