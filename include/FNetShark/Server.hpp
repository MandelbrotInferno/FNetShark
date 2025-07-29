#pragma once


#include <winsock2.h>



namespace FNetShark
{
	class Server
	{
	public:

		Server(const char* l_port);

		void Init();

		void Bind();

		void Listen();

		SOCKET Accept();

		void CleanUp();

	private:

		WSADATA m_wsaData{};
		addrinfo* m_resultGetAddrInfo{};
		SOCKET m_ipv4ListenSocket{ INVALID_SOCKET };
		const char* m_serverPort{};

	};
}