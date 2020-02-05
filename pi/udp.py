import socket
import json
import UsableMotor as m_s
import RPi.GPIO as GPIO
from threading import Thread
import subprocess
import signal
import os
from adafruit_motorkit import MotorKit
localIP     = "0.0.0.0"
localPort   = 20001
bufferSize  = 1024
# Create a datagram socket
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))
print("UDP server Bound and ready...")
#start gstreamer as subprocess

process = subprocess.Popen(["sh", "./start_stream_ip_is_viewer.sh"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
print("launched gstreamer")
#set up motor bindings

kit = MotorKit()
motors = m_s.UsableMotors(kit)
motors.set_throttle(motors.LEFT, 0.0)
motors.set_throttle(motors.RIGHT, 0.0)
print("Motors ready...")
# Listen for incoming datagrams
def pingresp():
    IP     = "0.0.0.0"
    port   = 20003
    buffer  = 1024
    response       = "PONG"
    responsebytes         = str.encode(response)
    print("ping function started...")
    # Create a datagram socket
    UDPPingSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    # Bind to address and ip
    UDPPingSocket.bind((IP, port))
    while(True):
        bytesAddressPair = UDPPingSocket.recvfrom(buffer)
        message = bytesAddressPair[0]
        address = bytesAddressPair[1]
        # Sending a reply to client
        UDPPingSocket.sendto(responsebytes, address)
    UDPPingSocket.close()
    print("ping echo Terminated")

    
def main():
    print("niceness: ", os.nice(0))
    os.nice(-10)
    print("new niceness: ", os.nice(0))
    while(True):
        try:
            bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
            message = bytesAddressPair[0]
            address = bytesAddressPair[1]
            #clientMsg = "Message from Client:{}".format(message)
            #clientIP  = "Client IP Address:{}".format(address)
            #print(clientMsg)
            #print(message.decode())
            #print(clientIP)
            data = message.decode()
            data = data[0:-1]
            #print(data)
            actions = json.loads(data)
            #was thinking if using bumpers on xbox-controller for throttle
            #and a reverse button
            motors.set_throttle(motors.LEFT, actions['l'])
            motors.set_throttle(motors.RIGHT, actions['r'])
        
        except(KeyboardInterrupt):
            break

    process.send_signal(signal.SIGINT)# send Ctrl-C signal
    stdout, stderr = process.communicate()
    print( )
    print("sent sigint to gstreamer: ", stderr.decode() + ";" + stdout.decode())
    motors.terminate()
    print("engines stopped")
    UDPServerSocket.close()
    print("Socket closed. Exiting program...")

if __name__ == '__main__':
    t = Thread(target = pingresp, args =(),daemon=True)
    t.start()
    main()
