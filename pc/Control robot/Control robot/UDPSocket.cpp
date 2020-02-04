#include "UDPSocket.h"
#pragma comment(lib, "Ws2_32.lib")

struct addrinfo* result = NULL, * ptr = NULL, hints;
// Helper struct, for addr_in info and stuff like that

UDPSocket::UDPSocket(int type)
{
	initWsa();

	sock = INVALID_SOCKET;

	sock = ::socket(AF_INET, type, 0);
	if (sock == INVALID_SOCKET)
	{
		throw std::string("creating socket failed: %d\n", WSAGetLastError());
	}
}
UDPSocket::UDPSocket(SOCKET s)
{
	sock = s;
}
static Address convert_to_address(sockaddr_in hint)
{
	char buffer[INET_ADDRSTRLEN];
	memset(buffer, 0, INET_ADDRSTRLEN);
	::inet_ntop(AF_INET, &hint.sin_addr, buffer, INET_ADDRSTRLEN);
	return{ std::string(buffer), ntohs(hint.sin_port) };
}


void UDPSocket::shutdown()
{
	int shutdown_result = ::shutdown(sock, SD_BOTH); // SD_SEND, SD_RECEIVE
	if (shutdown_result == SOCKET_ERROR)
	{
		throw std::string("shutdown failed with error %d\n", WSAGetLastError());
	}
}

void UDPSocket::close()
{
	::closesocket(sock);
}

Address UDPSocket::getAddress()
{
	sockaddr_in hint;
	socklen_t address_size = sizeof(hint);
	int result = ::getsockname(sock, (sockaddr*)&hint, &address_size);

	if (result == SOCKET_ERROR)
	{
		throw std::string("Error getting address/name %d\n", WSAGetLastError());
	}

	return convert_to_address(hint);
}

void UDPSocket::initWsa()
{
	int iResult;
	WSADATA wsaData;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		WSACleanup();
	}
}

void UDPSocket::quitWsa()
{
	WSACleanup();
}


UDPSocket::~UDPSocket()
{
	close();
	quitWsa();
}

void UDPSocket::connect(const Address& address)
{
	sockaddr_in address_in;
	address_in.sin_family = AF_INET;
	address_in.sin_port = htons(address.port);
	inet_pton(AF_INET, address.host.c_str(), &address_in.sin_addr);

	int bind_result = ::connect(sock, (sockaddr*)&address_in, sizeof(address_in));
	if (bind_result == SOCKET_ERROR)
	{
		throw std::string("connect failed with error %d\n", WSAGetLastError());
	}
}

void UDPSocket::bind(const Address& address)
{
	sockaddr_in address_in;
	address_in.sin_family = AF_INET;
	address_in.sin_port = htons(address.port);
	inet_pton(AF_INET, address.host.c_str(), &address_in.sin_addr);

	int bind_result = ::bind(sock, (sockaddr*)&address_in, sizeof(address_in));
	if (bind_result == SOCKET_ERROR)
	{
		throw std::string("bind failed with error %d\n", WSAGetLastError());
	}
}

void UDPSocket::listen()
{
	int listen_result = ::listen(sock, SOMAXCONN);
	if (listen_result == SOCKET_ERROR)
	{
		throw std::string("listen failed with error %d\n", WSAGetLastError());
	}
}

UDPSocket* UDPSocket::accept()
{
	SOCKET client_socket = ::accept(sock, (sockaddr*)NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		throw std::string("accept failed with error %d\n", WSAGetLastError());
	}
	return new UDPSocket(client_socket);
}

int UDPSocket::sendTo(const char* msg, int msgSize, const Address& address)
{

	sockaddr_in destination_adress;
	destination_adress.sin_family = AF_INET;
	destination_adress.sin_port = htons(address.port);
	inet_pton(AF_INET, address.host.c_str(), &destination_adress.sin_addr);

	int bytes_sent = ::sendto(sock, msg, msgSize, 0, (sockaddr*)&destination_adress, sizeof(destination_adress));

	if (bytes_sent == SOCKET_ERROR)
	{
		throw std::string("sendto failed with error %d\n", WSAGetLastError());
	}

	return bytes_sent;
}

int UDPSocket::sendTo(const std::string& msg, const Address& address)
{
	return sendTo(msg.data(), msg.size() + 1, address);
}

int UDPSocket::receiveFrom(char* buf, int bufSize, Address& address)
{
	sockaddr_in source_address;
	source_address.sin_family = AF_INET;
	source_address.sin_port = htons(address.port);
	inet_pton(AF_INET, address.host.c_str(), &source_address.sin_addr);
	socklen_t address_size = sizeof(source_address);

	int bytesRecived = ::recvfrom(sock, buf, bufSize, 0, (sockaddr*)&source_address, &address_size);

	if (bytesRecived == SOCKET_ERROR)
	{
		throw std::string("recvfrom failed with error %d\n", WSAGetLastError());
	}

	address = convert_to_address(source_address);

	return bytesRecived;
}
