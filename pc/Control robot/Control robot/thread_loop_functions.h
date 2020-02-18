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
std::string umap_to_json(std::unordered_map<std::string, float>& power);
void poll_controller_input(int controller_index, bool& running, Address robot_socket);
void ping_loop(bool& running, Address robot_socket);