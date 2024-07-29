#include "session.hpp"
#include "router.hpp"
#include <boost/beast/version.hpp>
#include <context.hpp>
#include <spdlog/spdlog.h>

void session::run()
{
    do_read();
}

void session::do_read()
{
    http::async_read
    (
        m_stream,
        m_buffer,
        m_req,
        beast::bind_front_handler
        (
            &session::on_read,
            shared_from_this()
        )
    );
}

void session::on_read(beast::error_code const& ec, std::size_t const bytes_sent)
{
    boost::ignore_unused(bytes_sent);

    if (ec)
    {
        SPDLOG_ERROR("session::on_read: {}", ec.message());
        return;
    }

    // Initialize context for this request.
    auto const ctx = std::make_shared<context>();
    context::set_context(ctx);

    route_key const rk{m_req.method(), m_req.target()};
    auto const handler = m_router->lookup_handler(rk);

    SPDLOG_INFO("Routing request.");
    do_write
    (
        handler->handle(std::move(m_req), std::move(m_res))
    );
}

void session::do_write(http::message_generator&& msg)
{
    bool keep_alive = msg.keep_alive();

    beast::async_write
    (
        m_stream,
        std::move(msg),
        beast::bind_front_handler
        (
            &session::on_write,
            shared_from_this(),
            keep_alive
        )
    );
}

void session::on_write
(
    bool const keep_alive,
    beast::error_code const& ec,
    std::size_t bytes_sent
)
{
    boost::ignore_unused(bytes_sent);

    if (ec)
    {
        SPDLOG_ERROR("session::on_write: {}", ec.message());
        return;
    }

    if (!keep_alive)
    {
        return do_close();
    }

    SPDLOG_INFO("Response sent.");

    do_read();
}

void session::do_close()
{
    beast::error_code ec;

    ec = m_stream.socket().shutdown(tcp::socket::shutdown_send, ec);

    if (ec)
    {
        SPDLOG_ERROR("session::do_close: {}", ec.message());
    }
}
