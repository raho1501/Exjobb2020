#pragma once
#define _WINSOCKAPI_  
#include <memory>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include "Controller.h"
#include "UDPSocket.h"
#define ROUND_OF(f, c) (((float)((int)((f) * (c))) / (c)))
void ping_loop(bool& running, Address robot_socket)
{
    std::unique_ptr<UDPSocket> ping_socket(new UDPSocket(SOCK_DGRAM));
    char msg[1024];
    while (running)
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        ping_socket->sendTo("ping", robot_socket);
        int resp = ping_socket->receiveFrom(msg, 1024, robot_socket);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        printf(msg);
        for (int i = 0; i < resp; i++)
            std::cout << msg[i];
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "  " << time_span.count() * 1000.0 << " milliseconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(5000));
    }
}

void poll_controller_input(int controller_index, bool& running, Address robot_socket)
{
    std::unordered_map<std::string, float> power;
    std::unique_ptr<Controller> Player(new Controller(controller_index));

    std::unique_ptr<UDPSocket> controller_socket(new UDPSocket(SOCK_DGRAM));
    while (running)
    {
        if (Player->is_connected())
        {
            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                std::cout << "A was pressed" << std::endl;
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                std::cout << "B was pressed" << std::endl;
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_X)
            {
                std::cout << "X was pressed" << std::endl;
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
            {
                std::cout << "Y was pressed" << std::endl;
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
            {
                break;
            }
            //std::cout << Player->get_controller_status().Gamepad.sThumbLY << std::endl; //left thumb stick y-axis, ranges from -32000 to 32000 in value
            //std::cout << Player->get_controller_status().Gamepad.sThumbRY << std::endl; //right thumb stick y-axis, ranges from -32000 to 32000 in value
            power["r"] = ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbRY / 32000.0), 100);
            power["l"] = ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbLY / 32000.0), 100);

            std::string outmsg = "{";
            for (const auto& p : power) {
                outmsg.append(p.first + ":" + std::to_string(p.second) + ",");
            }
            outmsg.erase(outmsg.end() - 1);
            outmsg.append("}");

            std::cout << outmsg << std::endl;

            controller_socket->sendTo(outmsg, robot_socket);
            std::this_thread::sleep_for(std::chrono::microseconds(10));

        }
        else
        {
            std::cout << "\n\tERROR! PLAYER - XBOX 360 Controller Not Found!\n";
            std::cout << "Press Any Key To Exit.";
            std::cin.get();
            break;
        }
    }

}