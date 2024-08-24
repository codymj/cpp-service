#include "app.hpp"
#include <boost/beast/core.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
#include <config_manager.hpp>
#include <exception>
#include <iostream>
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/sinks/JsonConsoleSink.h>
#include "listener.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

void app::initialize(std::filesystem::path const& path)
{
    load_configuration(path);
    init_logger();
}

void app::load_configuration(std::filesystem::path const& path)
{
    try
    {
        m_config_manager = std::make_unique<config_manager>(path);
    }
    catch (YAML::ParserException const& e)
    {
        std::cerr
            << "Error parsing configuration properties: "
            << e.what();
        std::exit(EXIT_FAILURE);
    }
    catch (YAML::BadConversion const& e)
    {
        std::cerr
            << "Error converting property from properties.yml: "
            << e.what();
        std::exit(EXIT_FAILURE);
    }
    catch (std::exception const& e)
    {
        std::cerr
            << "Unknown error while loading configuration properties: "
            << e.what();
        std::exit(EXIT_FAILURE);
    }
}

void app::init_logger()
{
    // Start the backend thread.
    quill::BackendOptions const backend_options;
    quill::Backend::start(backend_options);

    // Create a JSON sink.
    auto sink = quill::Frontend::create_or_get_sink<quill::JsonConsoleSink>("sink");

    m_logger = quill::Frontend::create_or_get_logger
    (
        "root",
        std::move(sink),
        "",
        "%H:%M:%S.%Qms",
        quill::Timezone::GmtTime
    );

    try
    {
        std::string const level = m_config_manager->app_log_level();

        if (level == "trace")
            m_logger->set_log_level(quill::LogLevel::TraceL1);
        else if (level == "warn")
            m_logger->set_log_level(quill::LogLevel::Warning);
        else if (level == "error")
            m_logger->set_log_level(quill::LogLevel::Error);
        else if (level == "critical")
            m_logger->set_log_level(quill::LogLevel::Critical);
        else
            m_logger->set_log_level(quill::LogLevel::Debug);
    }
    catch (std::exception& e)
    {
        LOG_CRITICAL(m_logger, "Error initializing logger: {error}", e.what());
        std::exit(EXIT_FAILURE);
    }
}

int app::main()
{
    // Create data store registry to inject into service registry.
    m_store_registry = std::make_unique<store_registry>
    (
        m_config_manager.get()
    );

    // Create service registry to inject into the router.
    m_service_registry = std::make_unique<service_registry>
    (
        m_store_registry.get()
    );

    // Create router which is passed to the listener.
    m_router = std::make_unique<router>(m_service_registry.get());

    // Create and start the HTTP server.
    try
    {
        // The io_context is required for all I/O.
        net::thread_pool ioc(m_config_manager->server_threads());

        // Create and launch a listening port.
        std::make_shared<listener>
        (
            ioc.get_executor(),
            tcp::endpoint
            {
                net::ip::make_address(m_config_manager->server_host()),
                m_config_manager->server_port()
            },
            std::move(m_router)
        )->run();

        LOG_INFO
        (
            m_logger,
            "Started application: {app} {version} {port}",
            m_config_manager->app_domain() + "." + m_config_manager->app_name(),
            m_config_manager->app_version(),
            m_config_manager->server_port()
        );

        // Capture SIGINT and SIGTERM to perform a clean shutdown.
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait
        (
            [&](beast::error_code const&, int)
            {
                LOG_INFO(m_logger, "Stopping I/O context.");
                ioc.stop();
            }
        );

        ioc.join();
    }
    catch (beast::system_error const& e)
    {
        LOG_CRITICAL(m_logger, "Server error: {error}", e.what());
        std::exit(EXIT_FAILURE);
    }

    LOG_INFO(m_logger, "Shutting down.");

    return EXIT_SUCCESS;
}
