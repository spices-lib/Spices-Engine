/**
* @file TcpServer_test.h.
* @brief The TcpServer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/TcpServer.h>
#include "Instrumentor.h"

namespace SpicesTest {

    class TcpServerTest
    {
    public:

        TcpServerTest(Spices::Net::EventLoop* loop, Spices::Net::InetAddress& addr)
            : m_Server(loop, addr)
            , m_Loop(loop)
        {

            m_Server.SetConnectionCallback([=](const Spices::Net::TcpConnectionPtr& connection) { onConnection(connection); });
            m_Server.SetMessageCallback([=](const Spices::Net::TcpConnectionPtr& connection, Spices::Net::Buffer* buf) { onMessage(connection, buf); });
        }

        void start()
        {
            m_Server.Start();
        }

    private:

        void onConnection(const Spices::Net::TcpConnectionPtr& connection)
        {
            if (connection->Connected())
            {

            }
            else
            {

            }
        }

        void onMessage(const Spices::Net::TcpConnectionPtr& connection, Spices::Net::Buffer* buf)
        {
            std::string msg = buf->RetrieveAllAsString();
            connection->Send(msg);
            connection->ShutDown();
        }

        Spices::Net::EventLoop* m_Loop;
        Spices::Net::TcpServer m_Server;
    };

    /**
    * @brief Testing Spices::Net::TcpServer.
    */
    TEST(TcpServer_test, TcpServer) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::Net::EventLoop loop;
        Spices::Net::InetAddress address(8000);

        TcpServerTest server(&loop, address);
        server.start();
        loop.Loop();
    }

}