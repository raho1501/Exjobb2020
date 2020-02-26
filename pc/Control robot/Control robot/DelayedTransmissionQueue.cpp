#include "DelayedTransmissionQueue.h"
#include <iostream>
DelayedTransmissionQueue::DelayedTransmissionQueue(double init_delay, Address socket_address): 
	delay(init_delay), socket_address(socket_address), 
	lil_sock(new UDPSocket(SOCK_DGRAM))
{}

void DelayedTransmissionQueue::enqueue(std::string outgoing_message)
{
	messages_to_send.push(std::make_pair(outgoing_message, std::chrono::high_resolution_clock::now()));
}

bool DelayedTransmissionQueue::check_if_ready()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> delay_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - messages_to_send.front().second);
	if (delay_diff.count() > delay)
	{
		std::cout << delay_diff.count() << std::endl;
		return true;
	}
	else
		return false;
}

bool DelayedTransmissionQueue::transmitt()
{
	if (check_if_ready())
	{
		lil_sock->sendTo(messages_to_send.front().first, socket_address);
		messages_to_send.pop();
		return true;
	}
	return false;
	
}

void DelayedTransmissionQueue::set_delay(double new_delay)
{
	delay = new_delay;
}

void DelayedTransmissionQueue::clear_queue()
{
	while (!messages_to_send.empty())
		messages_to_send.pop();
}

DelayedTransmissionQueue::~DelayedTransmissionQueue()
{
	lil_sock->close();
	while(!messages_to_send.empty())
		messages_to_send.pop();
	delete lil_sock;
}
