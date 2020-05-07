#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#define _WINSOCKAPI_
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

//! Address struct
/*!
*
*/
struct Address
{
	std::string host;
	int port;
};
//! UDPSocket class, to send messages to the network
class UDPSocket
{
public:
	UDPSocket() = delete;
	//! Constructor
	/*!
	*	\param type the socket type, either SOCK_STREAM OR SOCK_DGRAM. Use the latter one.
	*/
	explicit UDPSocket(int type);//SOCK_STREAM OR SOCK_DGRAM
	//! Destructor
	~UDPSocket();
	//! Connect function
	/*!
	* Used when in TCP mode, not needed in this context. Only included for convienience 
	*/
	void connect(const Address& address);
	//! Bind function
	/*!
	* Used when in TCP mode, not needed in this context. Only included for convienience
	*/
	void bind(const Address& address);
	//! Listen function
	/*!
	* Used when in TCP mode, not needed in this context. Only included for convienience
	*/
	void listen();
	//! accept function
	/*!
	* Used when in TCP mode, not needed in this context. Only included for convienience
	*/
	UDPSocket* accept();
	//! sendTo function, send UDP packet to address
	/*!
	*	Send message to socket address.
	*	\param msg string message to send
	*	\param address the address to send the message to
	*	\returns number of bytes sent
	*/
	int sendTo(const std::string& msg, const Address& address);
	//! sendTo function, send UDP packet to address
	/*!
	*	Send message to socket address.
	*	\param msg char array message to send
	*	\param msgSize length of the message to send
	*	\param address the address to send the message to
	*	\returns number of bytes sent
	*/
	int sendTo(const char* msg, int msgSize, const Address& address);
	//! receiveFrom function, accepts message from provided address
	/*!	accepts a message from provided address and stores it in a provided char buffer
	*	\param buf the buffer to store the received message
	*	\param bufSize the size of the buffer
	*	\param address the address to receive the message from
	*	\returns the length of the received message.
	*/
	int receiveFrom(char* buf, int bufSize, Address& address);
	//! shutdown the socket
	void shutdown();
	//! close the socket
	void close();
	//! get address from message
	Address getAddress();

private:
	//! init Wsa for windows
	/*! perfromes the request to the OS to get access to use the network
	*/
	static void initWsa();
	//! quit Wsa for windows
	static void quitWsa();
	//! private constructor
	/*! Constructor to create UDPSocket from existing windows Socket
	*	\param s the windows socket
	*/
	UDPSocket(SOCKET s);
	//! The socket
	SOCKET sock;
};

#endif