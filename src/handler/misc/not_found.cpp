#include "not_found.hpp"

http::message_generator not_found_handler::handle
(
    http::request<http::string_body> req,
    http::response<http::string_body> res
)
{
    res.chunked(true);
    res.result(http::status::not_found);
    res.set(http::field::server, "beast");
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = R"({"status":"not found"})";

    // Call next handler, if any.
    if (m_next)
    {
        return m_next->handle
        (
            std::move(req),
            std::move(res)
        );
    }

    res.prepare_payload();
    return res;
}
