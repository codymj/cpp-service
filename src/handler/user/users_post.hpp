#pragma once

#include "../../service/user/user_service.hpp"
#include <handler.hpp>

class users_post_handler final
: public handler
{
public:
    /**
     * Handler for POST /users.
     * @param user_service User service layer.
     * @param next Next handler in chain.
     */
    explicit users_post_handler
    (
        user_service* user_service,
        std::unique_ptr<handler> next = nullptr
    )
    : m_user_service(user_service)
    , m_next(std::move(next))
    {}

    /**
     * Handle method for POST /users.
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
     * Service layer for User.
     */
    user_service* m_user_service;

    /**
     * Next handler in the chain.
     */
    std::unique_ptr<handler> m_next = nullptr;
};
