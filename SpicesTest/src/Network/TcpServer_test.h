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

        TcpServerTest(Spices::Net::InetAddress& addr)
            : m_Server(addr)
        {
            m_Server.AddConnectionCallback([=](const Spices::Net::TcpConnectionPtr& connection) { onConnection(connection); });
            m_Server.AddMessageCallback([=](const Spices::Net::TcpConnectionPtr& connection, Spices::Net::Buffer* buf) { onMessage(connection, buf); });
        }

        void start()
        {
            m_Server.Start(1);
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

        Spices::Net::TcpServer m_Server;
    };

    /**
    * @brief Testing Spices::Net::TcpServer.
    */
    TEST(TcpServer_test, TcpServer) {

        SPICESTEST_PROFILE_FUNCTION();

        TcpServerTest server(Spices::Net::InetAddress(8000));
        server.start();
        Spices::Net::pTLSEventLoop.GetInst()->Loop();
    }

}