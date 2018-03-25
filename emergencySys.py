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
    print('Starting emergency service')
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(10, GPIO.IN, GPIO.PUD_DOWN)
    GPIO.setup(11, GPIO.IN, GPIO.PUD_DOWN)
    GPIO.setup(12, GPIO.IN, GPIO.PUD_DOWN)
    GPIO.setup(14, GPIO.OUT)
    GPIO.setup(15, GPIO.OUT)
    GPIO.output(14, GPIO.LOW)
    GPIO.output(15, GPIO.LOW)

    while run:
        try:
            url = cam_loc + '/photo.jpg'
            c = ""
            id = ""
            p = 0
            i = 0
            name = ""
            msg = ""
            while run:
                if GPIO.input(10) == 1:
                    msg = "POLICE"
                    break
                elif GPIO.input(11) == 1:
                    msg = "AMBULANCE"
                    break
                elif GPIO.input(12) == 1:
                    msg = "FIRE"
                    break
                time.sleep(.1)
            GPIO.output(14, GPIO.HIGH)
            print('Got from button: ' + msg)
            urlopen(cam_loc + '/ptz?zoom=5')
            scan_count = 0
            while run and scan_count < 5:
                scan_count += 1
                urlopen(cam_loc + '/focus')
                for i in range(5):
                    time.sleep(.5)
                    GPIO.output(14, GPIO.LOW)
                    time.sleep(.5)
                    GPIO.output(14, GPIO.HIGH)
                print('Asking IMG')
                imgresp = urlopen(url)
                imgnp = np.array(bytearray(imgresp.read()), dtype=np.uint8)
                print('Decoding IMG ' + str(len(imgnp) if imgnp is not None else None))
                start = time.time()
                img = cv2.imdecode(imgnp, -1)
                cv2.imwrite('2.png', img)
                image = decode(Image.open('2.png'))
                print(time.time() - start)
                print('Done Decode')
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
                for x in image:
                    c = c + str(x.data)
                    i = i + 1
                if i != 0:
                    print("Decoded Aadhar card")
                    break
            else:
                GPIO.output(14, GPIO.LOW)
                continue
            if run:
                o = "uid="
                f = c.index(o, 0, len(c))
                for i in range(f + 5, f + 17):
                    id = id + c[i]
                print("Decoded Aadhar number: ", end='')
                print(id)  # id has the aadhar number
                callback(id, msg)
                GPIO.output(14, GPIO.LOW)
                GPIO.output(15, GPIO.HIGH)
                time.sleep(1.5)
                GPIO.output(15, GPIO.LOW)
        except:
            pass
        GPIO.output(14, GPIO.LOW)
        GPIO.output(15, GPIO.LOW)
    print('Ending emergency service')
