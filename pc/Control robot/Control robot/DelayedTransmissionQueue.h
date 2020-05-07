#pragma once
#include <queue>
#include <chrono>
#include <utility>
#include <string>
#include <memory>
#include "UDPSocket.h"
//!  DelayedTransmissionQueue class
/*!
* a queue that will not dequeue a message and send it to the network unless a set time is passed. 
*/
class DelayedTransmissionQueue
{
public:
	DelayedTransmissionQueue() = delete;
	//! Constructor
	/*!
	* Takes 
		\param init_delay is the delay in seconds.
	    \param socket_address is a Address to send to.
	*/
	explicit DelayedTransmissionQueue(double init_delay, Address socket_address);
	//! enqueue message to the back of the queue
	void enqueue(std::string outgoing_message); //add messages to send
	//! transmitt function that sends the message at the front of the queue, returns if transmission was performed or not
	/*!
	* Function performs a check if a set time has passed. If true then it will send the first message and return true.
	* If the set time has not passed then no transmisson of message is performed and a false is returned.
	* Check is done by private function.
	*/
	bool transmitt();
	//! set delay of queue
	/*!
	* The \param new_delay is in unit seconds
	*/
	void set_delay(double new_delay);//seconds
	//! clear_queue
	void clear_queue();
	//!Destructor
	~DelayedTransmissionQueue();
private:
	//! check if set delay time has passed
	/*!
	* returns boolean corresponding to if the set delay has passed. Used by the transmitt function
	*/
	bool check_if_ready();
	//! The time to wait before transmission is allowed
	double delay;
	//! Socket to send message over udp with
	UDPSocket* lil_sock;
	//! address to the receiver
	Address socket_address;
	//! The queue that holds all messages and timestamps
	std::queue<std::pair<std::string, std::chrono::high_resolution_clock::time_point>> messages_to_send; //high_resolution_clock is fine on windows since its cast to steady_clock
};

