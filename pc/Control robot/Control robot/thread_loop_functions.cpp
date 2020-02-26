#include "thread_loop_functions.h"

#define ROUND_OF(f, c) (((float)((int)((f) * (c))) / (c)))
void ping_loop(bool& running, Address robot_socket)
{
    try{
        std::unique_ptr<UDPSocket> ping_socket(new UDPSocket(SOCK_DGRAM));
        char msg[1024];

        while (running)
        {
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            ping_socket->sendTo("ping", robot_socket);
            int resp = ping_socket->receiveFrom(msg, 1024, robot_socket);
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            //printf(msg);
            for (int i = 0; i < resp; i++)
                std::cout << msg[i];
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
            std::cout << "  " << time_span.count() * 1000.0 << " milliseconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        }
    }
    catch (...)
    {
        return;
    }
    std::cout << "Ping thread closed..." << std::endl;
}

std::string umap_to_json(std::unordered_map<std::string, float> &power) {
    std::string outmsg = "{";
    for (const auto& p : power) {
        outmsg.append('"' + p.first + '"' + ':' + std::to_string(p.second) + ",");
    }
    outmsg.erase(outmsg.end() - 1);
    outmsg.erase(outmsg.end() - 1);
    outmsg.append("}");
    //std::cout << "Length of message: " << std::endl << outmsg.size() << std::endl << std::endl;
    return outmsg;
}

float map_throttle_response(float throttle)
{
    float res;
    float input_start = 0.0; // The lowest number of the range input.
    float input_end = 1.0;
    float output_start = 0.3;
    float output_end = 1.0;
    if (throttle < -0.15)
    {
        throttle *= -1.0;
        res = (throttle - input_start) / (input_end - input_start) * (output_end - output_start) + output_start;
        res *= -1;
    }
    else if (throttle > 0.15)
    {
        res = (throttle - input_start) / (input_end - input_start) * (output_end - output_start) + output_start;
    }
    else
        res = 0.0;
        return res;
}

void poll_controller_input(int controller_index, bool& running, Address robot_socket)
{
    std::unordered_map<std::string, float> power;
    std::unique_ptr<Controller> Player(new Controller(controller_index));
    double delay_s = 0.0;
    //std::unique_ptr<UDPSocket> controller_socket(new UDPSocket(SOCK_DGRAM));
    DelayedTransmissionQueue controller_queue(0.0, robot_socket);
    while (running)
    {
        if (Player->is_connected())
        {
            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                delay_s += 0.01;//seconds, 0.01s = 10ms
                std::cout << "A was pressed, increasing delay. " << delay_s << " is the new delay" << std::endl;
                controller_queue.set_delay(delay_s);
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                delay_s -= 0.01;//seconds, 0.01s = 10ms
                std::cout << "B was pressed, increasing delay. " << delay_s << " is the new delay" << std::endl;
                controller_queue.clear_queue();
                controller_queue.set_delay(delay_s);
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_X)
            {
                delay_s = 0.000;//seconds, 0.01s = 10ms
                std::cout << "X was pressed, reseting delay. " << delay_s << " is the new delay" << std::endl;
                controller_queue.set_delay(delay_s);
                controller_queue.clear_queue();
                std::cout << "cleared the queue" << std::endl;

            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
            {
                std::cout << "Y was pressed, delay status: " << delay_s << " s, is the delay" << std::endl;
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
            {
                running = false;
                break;
            }
            //std::cout << Player->get_controller_status().Gamepad.sThumbLY << std::endl; //left thumb stick y-axis, ranges from -32000 to 32000 in value
            //std::cout << Player->get_controller_status().Gamepad.sThumbRY << std::endl; //right thumb stick y-axis, ranges from -32000 to 32000 in value
            power["r"] = map_throttle_response( max(min(ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbRY / 32000.0), 10), 1.00), -1.00) ); //clamp result between [-1 , 1]
            power["l"] = map_throttle_response( max(min(ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbLY / 32000.0), 10), 1.00), -1.00) ); //same here because the motors can only handle input in that range since it operates using pwm signals

            std::string outmsg = umap_to_json(power);

            //std::cout << outmsg << std::endl;
            controller_queue.enqueue(outmsg);
            bool sent = controller_queue.transmitt();
            //if (sent)
                //std::cout <<"Message on the way... "<< std::endl;
            //controller_socket->sendTo(outmsg, robot_socket);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));

        }
        else
        {
            std::cout << "\n\tERROR! PLAYER - XBOX 360 Controller Not Found!\n";
            std::cout << "Press Any Key To Exit.";
            std::cin.get();
            break;
        }
    }
    std::cout << "Controller thread closed..." << std::endl;
}