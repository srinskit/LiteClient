from time import sleep
import serial
from ws4py.client.threadedclient import WebSocketClient
import json
from multiprocessing import Queue
import threading
inQ = Queue()
outQ = Queue()
serverip = 'ws://10.100.33.248/'

class TermClient(WebSocketClient):
    cred = {}

    def __init__(self, path, cred):
        self.connected = False
        self.cred = cred
        path += '?username=' + cred['username'] + '&type=terminal'+'&cid='+cred['cid']
        WebSocketClient.__init__(self, path)

    @staticmethod
    def make_msg(typ, data):
        msg = {'type': typ, 'data': data}
        return json.dumps(msg)

    def process_msg(self, msg):
        typ = msg['type']
        if typ == 'auth':
            self.after_auth()
        elif typ == 'bs':
            #print(msg['data']['bs'])
            inQ.put(msg['data']['bs'])

    def opened(self):
        print('Connection Opened!')

    def after_auth(self):
        self.connected = True
        self.send(self.make_msg('addListener', {'loc': 'lamps', 'cid': self.cred['cid']}))

    def closed(self, code, reason=None):
        self.connected = False
        print('Connection Closed!', code, reason)

    def received_message(self, m):
        print(m)
        self.process_msg(json.loads(m.data.decode("utf-8")))

    def send(self, m):
        try:
            super().send(m)
        except:
            pass


def fake_it(bs):
    typ = bs[:5]
    if typ == '00010':
        bri = '00'
        #input('Bri of ' + str(int(bs[5:15], 2)) + ' ')
        outQ.put(bs[:15]+bri+bs[18:])


def soc_manager():
    global ws, cid, stopT
    retry = 0
    ws = TermClient(serverip, {'username': 'term1', 'cid': cid})
    while not stopT:
        if not ws.connected:
            while not stopT:
                try:
                    ws.connect()
                    retry = 0
                    break
                except:
                    retry = retry + 1
                    print('Socket connection failed. Try : ', retry)
                    del ws
                    ws = TermClient(serverip, {'username': 'term1', 'cid': cid})
                sleep(1)
            sleep(5)
        else:
            while not outQ.empty():
                ws.send(TermClient.make_msg('bs', {'bs': outQ.get()}))
    try:
        ws.close()
    except:
        pass

readTime = True
# main
try:
    cid = input('CID : ')
    ws = None
    stopT = False
    socThread = threading.Thread(target=soc_manager)
    socThread.start()
    while True:
        try:
            ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=10)
            while True:
                try:
                    if readTime:
                        ack = ser.read(29)
                        if ack == b'':
                            continue
                        print('Got from ard : '+ ack.decode("utf-8"))
                        readTime = False
                    else:
                        if not inQ.empty():
                            bs = inQ.get()
                            ser.write(bs.encode('ascii'))
                            fake_it(bs)
                            readTime = True
                except:
                    print('Couldn\'t send/recv to usb dev')
                    sleep(2)
                    break
            ser.close()
        except:
            print('Couldn\'t connect to usb dev')
            sleep(2)


except KeyboardInterrupt:
    stopT = True
    socThread.join()
    ser.close()
    quit()
