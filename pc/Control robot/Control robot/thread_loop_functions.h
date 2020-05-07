#pragma once
#define _WINSOCKAPI_ 
#include <thread>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "Controller.h"
#include "UDPSocket.h"
#include <chrono>
#include "DelayedTransmissionQueue.h"
//! Convertion function for unordered map to single-depth json-string
/*!
 *  Takes the unordered map of pairs containing the amount of throttle to each motor and converts it to a json-string that can be sent to the robot.
 *  \param power is a map containing the throttle amount to each engine
 *  \returns single-depth json-formated string
 */
std::string umap_to_json(std::unordered_map<std::string, float>& power);
//! Function that polls the controller for input and perfroms accordingly
/*!
*   Polls the controller and performs the actions that are registered.
    Will add the lever input to the transmission queue for sending to the robot at the given address.
    \param controller_index the index for the connected controller to read input from
    \param running the flag that defines if the polling loop should be running 
    \param robot_socket the socket address to the robot to be controlled
*/
void poll_controller_input(int controller_index, bool& running, Address robot_socket);
//! ping loop that sends a message to the robot and waits for response repeatedly
/*!
*   Function will ping the robot and write the RTT to a file called ping_times.txt at the same depth as the execution is performed.
    \param running boolean to check if the loop should continue, used to get a graceful program closure.
    \param robot_socket socket address to the robot. port will be different than for the controller loop.
*/
void ping_loop(bool& running, Address robot_socket);