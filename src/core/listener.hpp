#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/strand.hpp>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
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

        SPDLOG_INFO("Opening acceptor.");
        ec = m_acceptor.open(endpoint.protocol(), ec);
        if (ec)
        {
            SPDLOG_ERROR("Error opening acceptor: {}", ec.message());
            return;
        }

        SPDLOG_INFO("Setting reuse address on acceptor.");
        ec = m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            SPDLOG_ERROR("Error setting option: {}", ec.message());
            return;
        }

        SPDLOG_INFO("Binding acceptor.");
        ec = m_acceptor.bind(endpoint, ec);
        if (ec)
        {
            SPDLOG_ERROR("Error binding acceptor: {}", ec.message());
            return;
        }

        SPDLOG_INFO("Listening on acceptor.");
        ec = m_acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            SPDLOG_ERROR("Error listening on acceptor: {}", ec.message());
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

    net::any_io_executor m_executor;
    tcp::acceptor m_acceptor;
    std::unique_ptr<router> m_router;
};
