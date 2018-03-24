from pyzbar.pyzbar import decode
from PIL import Image
from urllib.request import urlopen
import numpy as np
import cv2
import time
import RPi.GPIO as GPIO

run = True
cam_loc = 'http://10.100.35.192:8080'


def exe(callback):
    global run
    GPIO.setwarnings(False)
    print('Starting emergency service')
    while run:
        try:
            # Extract from IP WEBCAM
            urlopen(cam_loc + '/ptz?zoom=5')
            url = cam_loc + '/photo.jpg'
            c = ""
            id = ""
            p = 0
            i = 0
            msg = ""
            name = ""
            while run:
                # urlopen(cam_loc + '/focus')
                time.sleep(5)
                print('Asking IMG')
                imgresp = urlopen(url)
                imgnp = np.array(bytearray(imgresp.read()), dtype=np.uint8)
                print('Decoding IMG ' + str(len(imgnp) if imgnp is not None else None))
                start = time.time()
                img = cv2.imdecode(imgnp, -1)
                cv2.imwrite('2.png', img)
                # cv2.imshow('1', img)
                # Decoding the Aadhar Qr code
                image = decode(Image.open('2.png'))
                print(time.time() - start)
                print('Done Decode')
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
                for x in image:
                    c = c + str(x.data)
                    i = i + 1
                if i != 0:
                    print("Decoded Aadhar card\n")
                    break
            if run:
                # print(c)
                # Extracting Aadhar number from string
                o = "uid="
                f = c.index(o, 0, len(c))
                for i in range(f + 5, f + 17):
                    id = id + c[i]
                print("Decoded Aadhar number: ", end='')
                print(id)  # id has the aadhar number

            while run:
                GPIO.setmode(GPIO.BCM)
                GPIO.setup(12, GPIO.OUT)
                GPIO.output(12, GPIO.HIGH)
                GPIO.setup(10, GPIO.IN, GPIO.PUD_DOWN)
                GPIO.setup(11, GPIO.IN, GPIO.PUD_DOWN)
                if GPIO.input(10) == 0:
                    msg = "POLICE"
                    callback(id, msg)
                    break
                elif GPIO.input(11) == 0:
                    msg = "AMBULANCE"
                    callback(id, msg)
                    break
                time.sleep(0)
            print(msg)
        except:
            pass
    print('Ending emergency service')
