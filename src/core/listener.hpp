#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/strand.hpp>
#include <quill/Frontend.h>
#include <quill/Logger.h>
#include <quill/LogMacros.h>

#include "router.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

/**
 * Listener for accepting requests and creating sessions.
 */
class listener
: public std::enable_shared_from_this<listener>
{
public:
    listener
    (
        net::any_io_executor const& executor,
        tcp::endpoint const& endpoint,
        std::unique_ptr<router> router
    )
    : m_executor(executor)
    , m_acceptor(make_strand(executor))
    , m_router(std::move(router))
    {
        beast::error_code ec;

        LOG_INFO(m_logger, "Opening acceptor.");
        ec = m_acceptor.open(endpoint.protocol(), ec);
        if (ec)
        {
            LOG_ERROR(m_logger, "Error opening acceptor: {error}", ec.message());
            return;
        }

        LOG_INFO(m_logger, "Setting reuse address on acceptor.");
        ec = m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            LOG_ERROR(m_logger, "Error setting option: {error}", ec.message());
            return;
        }

        LOG_INFO(m_logger, "Binding acceptor.");
        ec = m_acceptor.bind(endpoint, ec);
        if (ec)
        {
            LOG_ERROR(m_logger, "Error binding acceptor: {error}", ec.message());
            return;
        }

        LOG_INFO(m_logger, "Listening on acceptor.");
        ec = m_acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            LOG_ERROR(m_logger, "Error listening on acceptor: {error}", ec.message());
            return;
        }
    }

    /**
     * Runs the listener.
     */
    void run();

private:
    /**
     * Accept a request.
     */
    void do_accept();

    /**
     * Creates a session and forwards request.
     * @param ec Error code.
     * @param socket TCP socket for HTTP requests.
     */
    void on_accept(beast::error_code const& ec, tcp::socket socket);

    quill::Logger* m_logger = quill::Frontend::get_logger("root");
    net::any_io_executor m_executor;
    tcp::acceptor m_acceptor;
    std::unique_ptr<router> m_router;
};
