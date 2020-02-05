#pragma once
#include <queue>
#include <chrono>
#include <utility>
#include <string>
#include <memory>
#include "UDPSocket.h"

class DelayedTransmissionQueue
{
public:
	DelayedTransmissionQueue() = delete;
	explicit DelayedTransmissionQueue(double init_delay, Address socket_address);
	void enqueue(std::string outgoing_message); //add messages to send
	bool transmitt();
	void set_delay(double new_delay);//seconds
	~DelayedTransmissionQueue();
private:
	bool check_if_ready();
	double delay;
	UDPSocket* lil_sock;
	Address socket_address;
	std::queue<std::pair<std::string, std::chrono::high_resolution_clock::time_point>> messages_to_send;
};

