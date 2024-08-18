#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <quill/Frontend.h>

#include "router.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

/**
 * Session to be created upon receiving a request.
 */
class session
: public std::enable_shared_from_this<session>
{
public:
    explicit session(tcp::socket socket, router* router)
    : m_stream(std::move(socket))
    , m_router(router)
    {}

    /**
     * Begins a session.
     */
    void run();

private:
    /**
     * Reads a request.
     */
    void do_read();

    /**
     * Handles a request.
     * @param ec Error code.
     * @param bytes_sent Total bytes transferred.
     */
    void on_read(beast::error_code const& ec, std::size_t bytes_sent);

    /**
     * Writes a response back to the client.
     * @param msg Type-erased response.
     */
    void do_write(http::message_generator&& msg);

    /**
     * Logic to run after response has been sent. Typically, do another read.
     * @param keep_alive Keep connection alive?
     * @param ec Error code.
     * @param bytes_sent Total bytes transferred.
     */
    void on_write
    (
        bool keep_alive,
        beast::error_code const& ec,
        std::size_t bytes_sent
    );

    /**
     * Closes connection.
     */
    void do_close();

    /**
     * Socket stream.
     */
    beast::tcp_stream m_stream;

    /**
     * Read buffer.
     */
    beast::flat_buffer m_buffer;

    /**
     * HTTP request for this session.
     */
    http::request<http::string_body> m_req{};

    /**
     * HTTP response for this session.
     */
    http::response<http::string_body> m_res{};

    /**
     * Logger.
     */
    quill::Logger* m_logger = quill::Frontend::get_logger("root");

    /**
     * HTTP request router.
     */
    router* m_router;
};
