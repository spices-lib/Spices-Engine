/**
* @file Buffer_test.h.
* @brief The Buffer_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/Net/Buffer.h>
#include <Network/Net/Socket.h>
#include "Instrumentor.h"

namespace SpicesTest {

	class BufferTest
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

			Spices::Net::Buffer buffer;
			int error;
			for (int i = 0; i < 100; i++)
			{
				buffer.ReadFd(clientSocket.Fd(), &error);

				std::stringstream ss0;
				ss0 << "Hello Client! " << i;

				EXPECT_STREQ(buffer.RetrieveAllAsString().c_str(), ss0.str().c_str());

				std::stringstream ss1;
				ss1 << "Hello Server! " << i;

				buffer.Append(ss1.str());
				buffer.WriteFd(clientSocket.Fd(), &error);
				buffer.RetrieveAll();
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

			Spices::Net::Buffer buffer;
			int error;
			for (int i = 0; i < 100; i++)
			{
				std::stringstream ss0;
				ss0 << "Hello Client! " << i;

				buffer.Append(ss0.str());
				buffer.WriteFd(clientSocket.Fd(), &error);
				buffer.RetrieveAll();

				std::stringstream ss1;
				ss1 << "Hello Server! " << i;
				buffer.ReadFd(clientSocket.Fd(), &error);

				EXPECT_STREQ(buffer.RetrieveAllAsString().c_str(), ss1.str().c_str());
			}
		}
	};

    /**
    * @brief Testing Spices::Net::Read/Write.
    */
    TEST(Buffer_test, ReadWrite) {

        SPICESTEST_PROFILE_FUNCTION();

        const Spices::Net::Buffer buffer;

        EXPECT_EQ(buffer.ReadableBytes(), 0);
        EXPECT_EQ(buffer.WriteableBytes(), 1024);
        EXPECT_EQ(buffer.HeaderBytes(), 8);

		std::thread server([&]() {
			BufferTest::StartServer(Spices::Net::InetAddress(8000, "127.0.0.1"));
		});

		std::thread client([&]() {
			BufferTest::StartClient(Spices::Net::InetAddress(8000, "127.0.0.1"));
		});

		server.join();
		client.join();
    }

}