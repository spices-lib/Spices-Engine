/**
* @file Socket_test.h.
* @brief The Socket_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/Net/Socket.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class SocketTest
	{
	public:

		/**
		* @brief Create Server.
		* @param[in] address Server address.
		*/
		static void StartServer(const Spices::Net::InetAddress& address)
		{
			Spices::Net::Socket serverSocket;
			serverSocket.Create();

			serverSocket.BindAddress(address);
			serverSocket.Listen();

			Spices::Net::InetAddress peerAddress;
			Spices::Net::Socket clientSocket(serverSocket.Accept(&peerAddress));

			for (int i = 0; i < 100; i++)
			{
				std::string message = clientSocket.Receive();
				++m_NReceives;

				clientSocket.Send("Hello server!");
				++m_NSends;
			}
		}

		/**
		* @brief Create Client.
		* @param[in] address Client address.
		*/
		static void StartClient(Spices::Net::InetAddress& address)
		{
			Spices::Net::Socket clientSocket;
			clientSocket.Create();

			clientSocket.Connect(&address);

			for (int i = 0; i < 100; i++)
			{
				clientSocket.Send("Hello client!");
				++m_NSends;

				std::string response = clientSocket.Receive();
				++m_NReceives;
			}
		}

		static std::atomic_int m_NSends;
		static std::atomic_int m_NReceives;
	};

	std::atomic_int SocketTest::m_NSends = 0;
	std::atomic_int SocketTest::m_NReceives = 0;

	/**
	* @brief Testing Spices::Net::Server/Client.
	*/
	TEST(Socket_test, ServerClient) {

		SPICESTEST_PROFILE_FUNCTION();

		EXPECT_EQ(SocketTest::m_NSends, 0);
		EXPECT_EQ(SocketTest::m_NReceives, 0);

		std::thread server([&]() { 
			SocketTest::StartServer(Spices::Net::InetAddress(8000, "127.0.0.1"));
		});

		std::thread client([&]() { 
			SocketTest::StartClient(Spices::Net::InetAddress(8000, "127.0.0.1"));
		});
		
		server.join();
		client.join();

		EXPECT_EQ(SocketTest::m_NSends, 200);
		EXPECT_EQ(SocketTest::m_NReceives, 200);
	}

	/**
	* @brief Testing Spices::Net::ShutDownWrite.
	*/
	TEST(Socket_test, ShutDownWrite) {

		SPICESTEST_PROFILE_FUNCTION();

	}

	/**
	* @brief Testing Spices::Net::SetTcpNoDelay.
	*/
	TEST(Socket_test, SetTcpNoDelay) {

		SPICESTEST_PROFILE_FUNCTION();

	}

	/**
	* @brief Testing Spices::Net::SetReuseAddress.
	*/
	TEST(Socket_test, SetReuseAddress) {

		SPICESTEST_PROFILE_FUNCTION();

	}

	/**
	* @brief Testing Spices::Net::SetKeepAlive.
	*/
	TEST(Socket_test, SetKeepAlive) {

		SPICESTEST_PROFILE_FUNCTION();

	}
}