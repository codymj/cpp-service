#include "users_get.hpp"
#include "util/json_marshaller.hpp"
#include <spdlog/spdlog.h>

http::message_generator users_get_handler::handle
(
    http::request<http::string_body> req,
    http::response<http::string_body> res
)
{
    try
    {
        // Call to service to get some data.
        auto const users = m_user_service->getUsers();

        // No users to return.
        if (users->empty())
        {
            res.chunked(true);
            res.result(http::status::no_content);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = "";
        }
        else
        {
            // Build response.
            res.chunked(true);
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = JsonMarshaller::toJson(*users).dump();
        }
    }
    catch (std::exception const& e)
    {
        // TODO: Create JSON error message
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
