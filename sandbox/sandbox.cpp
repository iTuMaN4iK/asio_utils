﻿
#include "asio_ct/connection_tools.hpp"

using namespace eld;

int main()
{
    asio::io_context io_context{};

    auto runContext = [&io_context]()
    {
        std::string threadId{};
        std::stringstream ss;
        ss << std::this_thread::get_id();
        ss >> threadId;
        std::cout << std::string("New thread for asio context: ")
                     + threadId + "\n";
        std::cout.flush();

        io_context.run();

        std::cout << std::string("Stopping thread: ")
                     + threadId + "\n";
        std::cout.flush();

    };

    // auto workGuard = asio::make_work_guard(io_context);
//    ConnectionLogic connectionLogic{io_context,
//                                    {asio::ip::make_address_v4("127.0.0.1"),
//                                     12000}};

//    connectionLogic.setLogger([](const char *msg)
//                              {
//                                  std::cout << msg << std::endl;
//                              });

    asio::ip::tcp::socket client{io_context};

    asio::steady_timer timer{io_context};
    timer.expires_after(std::chrono::seconds(5));
    auto connectionAttempt = make_connection_attempt(client,
                                                     asio::steady_timer(io_context),
                                                     [](const asio::error_code &errorCode)
                                                     {
                                                         std::cout << errorCode.message() << std::endl;
                                                         return false;
                                                     });

    auto res = connectionAttempt(asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"),
                                                         12000),
                                 2);

    auto threads = {
            std::async(std::launch::async, runContext),
            std::async(std::launch::async, runContext)
    };

    // TODO: run server
    // TODO: refuse connections
    // TODO: accept connections
    // TODO: abort connection
    // TODO: connection time out

    return 0;}