/**
* @file Socket_test.h.
* @brief The Socket_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/Socket.h>
#include "Instrumentor.h"

namespace SpicesTest {

	static void InitializeWinSocket()
	{
		WSADATA wasData;
		if (WSAStartup(MAKEWORD(2, 2), &wasData) < 0)
		{
			SPICES_CORE_CRITICAL("InitializeWinSocket falied")
		}
	}

	static std::atomic_bool IsServerStarted = false;

	void StartServer(Spices::Net::InetAddress& address)
	{
		Spices::Net::Socket serverSocket;
		serverSocket.CreateNonBlocking();

		serverSocket.BindAddress(address);
		serverSocket.Listen();

		std::cout << "Server listening on: " << address.ToIPPort() << std::endl;
		IsServerStarted = true;

		Spices::Net::InetAddress peerAddress;
		Spices::Net::Socket clientSocket(serverSocket.Accept(&peerAddress));
		std::cout << "PeerAddress: " << peerAddress.ToIPPort() << std::endl;

		while (1) {}
	}

	void StartClient(Spices::Net::InetAddress& address)
	{
		Spices::Net::Socket clientSocket;
		clientSocket.CreateNonBlocking();

		clientSocket.Connect(&address);

		std::cout << "Connected to server: " << address.ToIPPort() << std::endl;

		clientSocket.Send("Hello client!");
		std::string response = clientSocket.Receive();
		std::cout << "Received: " << response << std::endl;
	}

	/**
	* @brief Testing Spices::Net::Socket.
	*/
	TEST(Socket_test, Socket) {

		SPICESTEST_PROFILE_FUNCTION();

		//InitializeWinSocket();

		std::thread server([&]() { 
			StartServer(Spices::Net::InetAddress(8000, "127.0.0.1")); 
		});

		std::thread client([&]() { 

			while (!IsServerStarted.load()) {}

			StartClient(Spices::Net::InetAddress(8000, "127.0.0.1")); 
		});
		
		server.join();
		client.join();
	}
}