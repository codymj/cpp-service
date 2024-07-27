#pragma once

#include <handler.hpp>

// TODO: Need to implement structured logging with JSON.
class logger_middleware final
: public handler
{
public:
    /**
     * Middleware for handling logging.
     * @param next
     */
    explicit logger_middleware(std::unique_ptr<handler> next = nullptr)
    : m_next(std::move(next))
    {}

    /**
     * Middleware handler for logging data.
     * @param req boost::beast::http::request<http::string_body>
     * @param res boost::beast::http::response<http::string_body>
     */
    http::message_generator handle
    (
        http::request<http::string_body> req,
        http::response<http::string_body> res
    ) override;

private:
    /**
     * Next handler in the chain, if any.
     */
    [[maybe_unused]] std::unique_ptr<handler> m_next = nullptr;
};
