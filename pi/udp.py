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

#set up motor bindings
kit = MotorKit()
motors = m_s.UsableMotors(kit) #use abstracted motor class
motors.set_throttle(motors.LEFT, 0.0)
motors.set_throttle(motors.RIGHT, 0.0)
print("Motors ready...")

# Function to listen for incoming datagrams
def pingresp():
    IP     = "0.0.0.0" #accept all connections
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
    #main loop for motors
    while(True):
        try:
            bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
            message = bytesAddressPair[0]
            address = bytesAddressPair[1]
            data = message.decode()
            data = data[0:-1] #Remove null-str from c-input c++ code sends null terminated c-string
            actions = json.loads(data) #Load json to dict
            motors.set_throttle(motors.LEFT, actions['l'])
            motors.set_throttle(motors.RIGHT, actions['r'])
        
        except(KeyboardInterrupt):
            break
    #Loop exit, stop motors
    motors.terminate()
    print("engines stopped")
    #Close socket
    UDPServerSocket.close()
    print("Socket closed. Exiting program...")

if __name__ == '__main__':
    t = Thread(target = pingresp, args =(),daemon=True) #Thread for ping response function
    t.start() #Start thread
    main() # Run main loop
