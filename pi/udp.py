import socket
import json
import UsableMotor as m_s
import RPi.GPIO as GPIO
localIP     = "0.0.0.0"
localPort   = 20000
bufferSize  = 1024
msgFromServer       = "Hello UDP Client"
bytesToSend         = str.encode(msgFromServer)
# Create a datagram socket
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))
print("UDP server Bound and ready...")
#start gstreamer as subprocess
import subprocess
import signal
process = subprocess.Popen(["sh", "./start_stream_ip_is_viewer.sh"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
print("launched gstreamer")
#set up motor bindings
from adafruit_motorkit import MotorKit
kit = MotorKit()
motors = m_s.UsableMotors(kit)
motors.set_throttle(motors.LEFT, 0.0)
motors.set_throttle(motors.RIGHT, 0.0)
print("Motors ready...")
# Listen for incoming datagrams
while(True):
    try:
        bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
        message = bytesAddressPair[0]
        address = bytesAddressPair[1]
        clientMsg = "Message from Client:{}".format(message)
        clientIP  = "Client IP Address:{}".format(address)
        print(clientMsg)
        print(message.decode())
        print(clientIP)
        actions = json.loads(message.decode())
        #was thinking if using bumpers on xbox-controller for throttle
        #and a reverse button
        if actions['b'] == 0:
            motors.set_throttle(motors.LEFT, actions['l'])
            motors.set_throttle(motors.RIGHT, actions['r'])
        else:
            motors.set_throttle(motors.LEFT, -1*actions['l'])
            motors.set_throttle(motors.RIGHT, -1*actions['r'])
        
    except(KeyboardInterrupt):
        break

process.send_signal(signal.SIGINT)# send Ctrl-C signal
stdout, stderr = process.communicate()
print( )
print("sent sigint to gstreamer", stderr)

motors.terminate()
print("engines stopped")
UDPServerSocket.close()
print("Socket closed. Exiting program...")
