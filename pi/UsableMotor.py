from adafruit_motorkit import MotorKit
from time import sleep
from enum import Enum

class UsableMotors:
    LEFT = 1
    RIGHT = 2
    def __init__(self,Adafruit_kit):
        self.LeftMotor = Adafruit_kit.motor1
        self.RightMotor = Adafruit_kit.motor2

    def set_throttle(self,selected_motor, throttle_f):
        if selected_motor == self.LEFT:
            self.LeftMotor.throttle = throttle_f
        elif selected_motor == self.RIGHT:
            self.RightMotor.throttle = throttle_f

    def terminate(self):
        self.LeftMotor.throttle = 0
        self.RightMotor.throttle = 0

