import serial
import time

port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=1.0)

rainbow = open('rainbow', 'r')
bluered = open('blue_red', 'r')
greenred = open('green_red', 'r')


def show_ledstrip( leddata ):
    port.write("leddata\r")
    port.write(leddata)
    return

def show_animation():
    show_ledstrip(rainbow.read())
    time.sleep(0.05)
    show_ledstrip(bluered.read())
    time.sleep(0.05)
    show_ledstrip(greenred.read())
    time.sleep(0.05)

show_animation()

port.write("start\r")
