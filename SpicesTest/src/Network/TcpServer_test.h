/**
* @file TcpServer_test.h.
* @brief The TcpServer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/TcpServer.h>
#include <Network/EventLoop.h>
#include "Instrumentor.h"

namespace SpicesTest {

    class TcpServerTest
    {
    public:

        TcpServerTest(const Spices::Net::InetAddress& addr)
            : m_Server(addr)
        {
            m_Server.AddConnectionCallback([=](const Spices::Net::TcpConnectionPtr& connection) { onConnection(connection); });
            m_Server.AddMessageCallback([=](const Spices::Net::TcpConnectionPtr& connection, Spices::Net::Buffer* buf) { onMessage(connection, buf); });
        }

        void start()
        {
            m_Server.Start(3);
        }

    private:

        void onConnection(const Spices::Net::TcpConnectionPtr& connection)
        {
            if (connection->Connected())
            {
                std::stringstream ss;
                ss << "TcpServer Connection Up :" << connection->PeerAddress().ToIPPort();

                SPICES_CORE_INFO(ss.str())
            }
            else
            {
                std::stringstream ss;
                ss << "TcpServer Connection Down :" << connection->PeerAddress().ToIPPort();

                SPICES_CORE_INFO(ss.str())
            }
        }

        void onMessage(const Spices::Net::TcpConnectionPtr& connection, Spices::Net::Buffer* buf)
        {
            std::string msg = buf->RetrieveAllAsString();
            std::cout << msg << std::endl;
            connection->Send(msg);
            //connection->ShutDown();
        }

        Spices::Net::TcpServer m_Server;
    };

    /**
    * @brief Testing Spices::Net::TcpServer.
    */
    TEST(TcpServer_test, TcpServer) {

        SPICESTEST_PROFILE_FUNCTION();

        using namespace Spices::Net;

        InetAddress address(8000, "127.0.0.1");
        EventLoop* loop = nullptr;
        std::thread t([&]() {

            TcpServerTest server(address);
            server.start();
            
            loop = EventLoopThreadWrapper::GetInst();
            loop->Loop();
        });

        while (!loop) {}

        Socket client;
        client.Create();
        Channel channel(client.Fd(), loop);
        channel.EnableWriting();
        client.Connect(&address);
        client.Send("Hello client!");

        t.detach();
    }

}