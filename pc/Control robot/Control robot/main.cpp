#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <gst/gst.h>
#include <thread>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "Controller.h"
#include "UDPSocket.h"
#include <chrono>
#include "DelayedTransmissionQueue.h"
//ports: controller, 20001 | gstreamer, 20002 | ping_loop, 20003
#define ROUND_OF(f, c) (((float)((int)((f) * (c))) / (c)))


void gstream_method(int argc, char* argv[])
{
    GstElement* pipeline;
    GstBus* bus;
    GstMessage* msg;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Build the pipeline */
    //pipeline = gst_parse_launch("playbin uri=https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    //pipeline = gst_parse_launch("playbin uri=file:///C:/Users/Rasmus/Desktop/WRMMM.mp4", NULL);
    pipeline = gst_parse_launch("udpsrc port=20002 ! application/x-rtp, encoding-name=H264,payload=96 ! rtph264depay ! h264parse ! avdec_h264 ! autovideosink", NULL);

    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    /* Free resources */
    if (msg != NULL)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}

void ping_loop(bool &running, Address robot_socket)
{
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

std::string umap_to_json(std::unordered_map<std::string, float> power) {
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

void poll_controller_input(int controller_index, bool &running, Address robot_socket)
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
                delay_s+= 0.01;//seconds, 0.01s = 10ms
                std::cout << "A was pressed" << delay_s <<" is new delay"<< std::endl;
                controller_queue.set_delay(delay_s);
            }

            if (Player->get_controller_status().Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                std::cout << "B was pressed" << delay_s << " is the delay" << std::endl;
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
            power["r"] = max( min(ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbRY / 32000.0) , 100) , 1.00), -1.00); //clamp result between [-1 , 1]
            power["l"] = max( min(ROUND_OF(float(Player->get_controller_status().Gamepad.sThumbLY / 32000.0) , 100) , 1.00), -1.00); //same here because the motors can only handle input in that range since it operates using pwm signals
            
            std::string outmsg = umap_to_json(power);
            
            //std::cout << outmsg << std::endl;
            controller_queue.enqueue(outmsg);
            bool sent = controller_queue.transmitt();
            if(sent)
                std::cout <<"Message on the way... "<< std::endl;
            //controller_socket->sendTo(outmsg, robot_socket);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

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

int main(int argc, char* argv[])
{
    
    Address rpi = { "192.168.254.141", 20001 };
    bool running = true;
    std::thread controller_thread(poll_controller_input, 0, std::ref(running), rpi);
    //controller_thread.detach();
    rpi.port += 2;
    std::thread ping_thread(ping_loop, std::ref(running), rpi);
    //ping_thread.detach();
    
    gstream_method(argc, argv); //blocking in main loop. when stream ends, rest of the threads are closed.
    running = false;
    

    std::this_thread::sleep_for(std::chrono::seconds(1));
    controller_thread.join();
    ping_thread.join();
    std::cout << "Done..." << std::endl;
    return 0;
}