#pragma once

#include <handler.hpp>

class not_found_handler final
: public handler
{
public:
    /**
     * Handler for returning 404 statuses.
     * @param next Next handler in chain.
     */
    explicit not_found_handler(std::unique_ptr<handler> next = nullptr)
    : m_next(std::move(next))
    {}

    /**
     * Handler to return 404 error.
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
