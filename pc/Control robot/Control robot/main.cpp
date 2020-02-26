#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <gst/gst.h>
#include <thread>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "Controller.h"
#include "UDPSocket.h"
#include <chrono>
#include "DelayedTransmissionQueue.h"

#include "thread_loop_functions.h"
//ports: controller, 20001 | gstreamer, 20002 | ping_loop, 20003
#define ROUND_OF(f, c) (((float)((int)((f) * (c))) / (c)))

void netcat_stream_receiver(bool &running)
{
    /*
    FILE* pPipe;
    pPipe = _popen("c:\\Users\\Rasmus\\Desktop\\netcat-1.11\\nc64.exe -L -p 20002 | c:\\Users\\Rasmus\\Desktop\\mplayer-x86_64\\mplayer.exe -fps 200 -demuxer h264es -", "rt");
    if (pPipe == NULL)
    {
        running = false;
    }
    */
    std::system("c:\\Users\\Rasmus\\Desktop\\netcat-1.11\\nc64.exe -l -p 20002 | c:\\Users\\Rasmus\\Desktop\\mplayer-x86_64\\mplayer.exe -fps 200 -demuxer h264es -noidle -nojoystick -quiet -");
    std::cout << "stream over...\n\r cleaning up streaming elements..." << std::endl;
    running = false;
}


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

int main(int argc, char* argv[])
{
    
    Address rpi = { "192.168.254.189", 20003 };
    bool running = true;
    std::thread ping_thread(ping_loop, std::ref(running), rpi);
    //std::thread controller_thread(poll_controller_input, 0, std::ref(running), rpi);
    //controller_thread.detach();
    rpi.port -= 2;
    //ping_thread.detach();
    std::thread video_thread(netcat_stream_receiver, std::ref(running));
    video_thread.detach();
    poll_controller_input(0, running, rpi);
    //gstream_method(argc, argv); //blocking in main loop. when stream ends, rest of the threads are closed.
    running = false;
    

    std::this_thread::sleep_for(std::chrono::seconds(1));
    //controller_thread.join();
    ping_thread.join();
    std::cout << "Done..." << std::endl;
    return 0;
}