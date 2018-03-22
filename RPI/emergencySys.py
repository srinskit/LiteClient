from pyzbar.pyzbar import decode
from PIL import Image
import urllib
import numpy as np
import cv2
import time
import RPi.GPIO as GPIO

run = True


def exe(callback):
    global run

    print('Starting emergency service')
    while run:
        # Extract from IP WEBCAM
        urllib.urlopen('http://10.100.34.109:8080/ptz?zoom=5')
        urllib.urlopen('http://10.100.34.109:8080/focus')
        url = 'http://10.100.34.109:8080/photo.jpg'
        c = ""
        id = ""
        p = 0
        i = 0
        msg = ""
        name = ""
        while run:
            time.sleep(2)
            imgresp = urllib.urlopen(url)
            imgnp = np.array(bytearray(imgresp.read()), dtype=np.uint8)
            img = cv2.imdecode(imgnp, -1)
            cv2.imwrite('2.png', img)
            # cv2.imshow('1', img)

            # Decoding the Aadhar Qr code

            image = decode(Image.open('2.png'))
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
            for x in image:
                c = c + x.data
                i = i + 1
            if i != 0:
                print("Decoded Aadhar card\n")
                break
        if run:
            print(c)

            # Extracting Aadhar number from string

            o = "uid="

            f = c.index(o, 0, len(c))

            for i in range(f + 5, f + 17):
                id = id + c[i]

            print("Decoded Aadhar number:")
            print(id)  # id has the aadhar number
            print("\n")

        while run:
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(10, GPIO.OUT)
            GPIO.output(10, GPIO.HIGH)
            GPIO.setup(8, GPIO.IN, GPIO.PUD_DOWN)
            GPIO.setup(9, GPIO.IN, GPIO.PUD_DOWN)
            if GPIO.input(8) == LOW:
                msg = "POLICE"
                callback(id, msg)
                break
            elif GPIO.input(9) == LOW:
                msg = "AMBULANCE"
                callback(id, msg)
                break
    print('Ending emergency service')
