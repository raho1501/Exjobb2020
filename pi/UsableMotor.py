from adafruit_motorkit import MotorKit
from time import sleep
from enum import Enum
#Motorkit class for simpler use
class UsableMotors:
    LEFT = 1
    RIGHT = 2
    #Constructor, maps motorkit output ports on raspi-hat for control. 
    #put left motor on output 1 and right motor on output 2 when making connections.
    def __init__(self,Adafruit_kit):
        self.LeftMotor = Adafruit_kit.motor1
        self.RightMotor = Adafruit_kit.motor2
    #Set throttle to motor, range is [-1.0; 1.0], negative values results in a backwards spin. 
    # Throttle is in float.
    def set_throttle(self,selected_motor, throttle_f):
        if selected_motor == self.LEFT:
            self.LeftMotor.throttle = throttle_f
        elif selected_motor == self.RIGHT:
            self.RightMotor.throttle = throttle_f
    #Stopping function, 
    def terminate(self):
        self.LeftMotor.throttle = 0
        self.RightMotor.throttle = 0

