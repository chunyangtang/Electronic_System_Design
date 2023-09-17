# Description: This script is used to connect to the serial port and send MNIST images to the microcontroller.
# It is expected to be run on a PC as the main function.

import serial
from image_handler import pygame_interactive
import threading
import time

class BColors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

is_running = True

def read_serial(ser: serial.Serial):
    global is_running
    while is_running:
        # Read data from the serial port
        data = ser.readline().decode().strip()
        
        # Process the received data
        if data:
            print(BColors.WARNING + 'Received:' + BColors.ENDC, BColors.OKGREEN + data + BColors.ENDC)  # You can perform any desired operations with the received data

def main():
    global is_running
    try: 
        ser = serial.Serial('COM13', 115200, timeout=1)
        # Start a separate thread to read data from the serial port
        read_thread = threading.Thread(target=read_serial, args=(ser,), daemon=True)
        read_thread.start()
        while True:
            img_list = pygame_interactive()
            if img_list:
                ser.write("h".encode())
                # ser.write(" ".join(map(str, img_list)).encode())
                time.sleep(0.1)
                ser.write(bytes(img_list))
                print(BColors.OKBLUE + "Image Sent." + BColors.ENDC)
            # input()
    except KeyboardInterrupt:
        ser.write("q".encode())
        time.sleep(0.1)
        is_running = False
        read_thread.join()
        # Close the serial port on Ctrl+C
        ser.close()


if __name__ == '__main__':
    main()