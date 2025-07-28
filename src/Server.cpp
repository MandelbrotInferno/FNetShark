


#include "FNetShark/Server.hpp"
#include "FNetShark/Utils/WinSocketError.hpp"
#include <ws2tcpip.h>
#include <fmt/core.h>

#pragma comment(lib, "Ws2_32.lib")

namespace FNetShark
{

	Server::Server(const char* l_port)
		:m_serverPort(l_port)
	{

	}


	void Server::Init()
	{
		using namespace Utils;

		WINSOCKET_CHECK(WSAStartup(MAKEWORD(2,2), &m_wsaData));

		addrinfo lv_hint{};
		lv_hint.ai_family = AF_INET;
		lv_hint.ai_flags = AI_PASSIVE;
		lv_hint.ai_socktype = SOCK_STREAM;
		lv_hint.ai_protocol = IPPROTO_TCP;

		WINSOCKET_CHECK(getaddrinfo(nullptr, m_serverPort, &lv_hint, &m_resultGetAddrInfo));
		
		m_ipv4ListenSocket = socket(m_resultGetAddrInfo->ai_family, m_resultGetAddrInfo->ai_socktype, m_resultGetAddrInfo->ai_protocol);

		if (INVALID_SOCKET == m_ipv4ListenSocket) {
			fmt::print("socket() failed: {}\n", WSAGetLastError());
			throw "Serious failure occured!\n";
		}
	}


	void Server::Bind()
	{
		auto lv_result = bind(m_ipv4ListenSocket, m_resultGetAddrInfo->ai_addr, (int)m_resultGetAddrInfo->ai_addrlen);

		if (SOCKET_ERROR == lv_result) {
			fmt::print("bind() failed: {}\n", WSAGetLastError());
			throw "Serious failure occured!\n";
		}

		freeaddrinfo(m_resultGetAddrInfo);
		m_resultGetAddrInfo = nullptr;
	}


	void Server::Listen()
	{
		if (SOCKET_ERROR == listen(m_ipv4ListenSocket, SOMAXCONN)) {
			fmt::print("listen() failed: {}\n", WSAGetLastError());
			throw "Serious failure occured!\n";
		}
	}


	void Server::InitCleanUp()
	{
		using namespace Utils;

		if (nullptr != m_resultGetAddrInfo) {
			freeaddrinfo(m_resultGetAddrInfo);
		}

		if (INVALID_SOCKET != m_ipv4ListenSocket) {
			WINSOCKET_CHECK(closesocket(m_ipv4ListenSocket));
		}

		WINSOCKET_CHECK(WSACleanup());
	}

}