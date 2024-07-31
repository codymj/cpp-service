#pragma once

#include <handler.hpp>

/**
 * Handler for returning 404 statuses.
 */
class not_found_handler final
: public handler
{
public:
    /**
     * Constructor.
     * @param next Next handler in chain.
     */
    explicit not_found_handler(std::unique_ptr<handler> next = nullptr)
    : m_next(std::move(next))
    {}

    /**
     * Handler.
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
     * Next handler in the chain.
     */
    std::unique_ptr<handler> m_next = nullptr;
};
