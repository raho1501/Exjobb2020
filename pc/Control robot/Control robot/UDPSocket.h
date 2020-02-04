#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#define _WINSOCKAPI_
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>


#pragma comment(lib, "Ws2_32.lib")


struct Address
{
	std::string host;
	int port;
};

class UDPSocket
{
public:
	UDPSocket() = delete;
	explicit UDPSocket(int type);//SOCK_STREAM OR SOCK_DGRAM
	~UDPSocket();

	void connect(const Address& address);
	void bind(const Address& address);
	void listen();
	UDPSocket* accept();

	int sendTo(const std::string& msg, const Address& address);
	int sendTo(const char* msg, int msgSize, const Address& address);

	int receiveFrom(char* buf, int bufSize, Address& address);

	void shutdown();
	void close();
	Address getAddress();

private:

	static void initWsa();
	static void quitWsa();
	UDPSocket(SOCKET s);
	SOCKET sock;
};

#endif