#include "listener.hpp"
#include "router.hpp"
#include "session.hpp"

void listener::run()
{
    do_accept();
}

void listener::do_accept()
{
    m_acceptor.async_accept
    (
        make_strand(m_executor),
        beast::bind_front_handler
        (
            &listener::on_accept,
            shared_from_this()
        )
    );
}

void listener::on_accept(beast::error_code const& ec, tcp::socket socket)
{
    if (ec)
    {
        SPDLOG_ERROR("listener::on_accept: {}", ec.message());
        return;
    }

    SPDLOG_INFO("Accepted new connection.");
    std::make_shared<session>
    (
        std::move(socket),
        m_router.get()
    )->run();

    // Continue accepting new connections.
    do_accept();
}
