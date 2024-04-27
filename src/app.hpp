#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>

using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;

class App : public Poco::Util::ServerApplication {
protected:
    [[maybe_unused]] void initialize(Application& self) override;
    [[maybe_unused]] void uninitialize() override;
    int main(const std::vector<std::string>& args) override;
};
