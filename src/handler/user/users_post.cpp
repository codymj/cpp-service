#include "users_post.hpp"
#include "util/json_marshaller.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

http::message_generator users_post_handler::handle
(
    http::request<http::string_body> req,
    http::response<http::string_body> res
)
{
    // TODO: JSON validation middleware.

    try
    {
        // Parse request body into a User object.
        User const userToSave = JsonMarshaller::toUser(req);

        // Pass to service.
        m_user_service->saveUser(userToSave);

        // Build response.
        res.chunked(true);
        res.result(http::status::created);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = "";
    }
    catch (std::runtime_error const& e)
    {
        SPDLOG_ERROR("{}", e.what());
        res.chunked(true);
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        res.body() = "";
    }

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
