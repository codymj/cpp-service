#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <spdlog/async.h>
#include "router.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class session
: public std::enable_shared_from_this<session>
{
public:
    explicit session(tcp::socket socket, router* router)
    : m_stream(std::move(socket))
    , m_router(router)
    {}

    void run();

private:
    void do_read();
    void on_read(beast::error_code const& ec, std::size_t bytes_sent);
    void send_response(http::message_generator&& msg);
    void on_write
    (
        bool keep_alive,
        beast::error_code const& ec,
        std::size_t bytes_sent
    );
    void do_close();

    beast::tcp_stream m_stream;
    beast::flat_buffer m_buffer;
    http::request<http::string_body> m_req{};
    http::response<http::string_body> m_res{};
    router* m_router;
};
