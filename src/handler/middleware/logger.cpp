#include "logger.hpp"
#include <context.hpp>
#include <spdlog/spdlog.h>

http::message_generator logger_middleware::handle
(
    http::request<http::string_body> req,
    http::response<http::string_body> res
)
{
    // Get context from thread local storage.
    auto const ctx = context::get_context();

    // Log request.
    SPDLOG_INFO
    (
        "{} {} {}",
        ctx->trace_id(),
        req.method_string().data(),
        req.target().data()
    );

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
