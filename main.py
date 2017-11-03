from time import sleep
import serial
from ws4py.client.threadedclient import WebSocketClient
import json
from multiprocessing import Queue
import threading
import sys

inQ = Queue()
outQ = Queue()
server_config = {}
soc_thread = None
ws = None
stopT = False
cid = None
serial_dev = None


def main():
    s_config = open('server.config')
    for line in s_config:
        pair = line.split(':', 1)
        server_config[pair[0]] = pair[1]
    if server_config.get('ip') is None:
        print('No server')
        quit(1)
    time_to_read = True
    global soc_thread, ws, stopT, cid, serial_dev
    try:
        # cid = '1'
        cid = str(int(sys.argv[1]))
    except (IndexError, ValueError) as e:
        print('Invalid cid; Exception : ' + str(e))
        quit(1)
    soc_thread = threading.Thread(target=soc_manager)
    soc_thread.start()
    try:
        while True:
            try:
                serial_dev = serial.Serial('/dev/ttyUSB0', 9600, timeout=10)
                while True:
                    try:
                        if time_to_read:
                            ack = serial_dev.read(29)
                            if ack == b'':
                                continue
                            print('Got from ard : ' + ack.decode("utf-8"))
                            time_to_read = False
                        else:
                            if not inQ.empty():
                                bs = inQ.get()
                                serial_dev.write(bs.encode('ascii'))
                                fake_it(bs)
                                time_to_read = True
                    except:
                        print('Send/Receive to/from serial dev failed')
                        sleep(2)
                        break
            except:
                print('\nSerial dev not connected')
                sleep(2)
            finally:
                if serial_dev is not None:
                    serial_dev.close()
    except KeyboardInterrupt:
        stopT = True
        soc_thread.join()
        if serial_dev is not None:
            serial_dev.close()
        quit()


class TermClient(WebSocketClient):
    cred = {}

    def __init__(self, path, cred):
        self.connected = False
        self.cred = cred
        path += '?username=' + cred['username'] + '&type=terminal' + '&cid=' + cred['cid']
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
        json_msg = json.loads(m.data.decode("utf-8"))
        print('From srv : ' + str(json_msg))
        self.process_msg(json_msg)

    def send(self, m, **kwargs):
        print('To srv : ' + str(m))
        try:
            super().send(m)
        except:
            pass


def fake_it(bs):
    typ = bs[:5]
    if typ == '00010':
        bri = '00'
        # input('Bri of ' + str(int(bs[5:15], 2)) + ' ')
        outQ.put(bs[:15] + bri + bs[18:])


def soc_manager():
    global ws, cid, stopT
    retry = 0
    ws = TermClient(server_config['ip'], {'username': 'term1', 'cid': cid})
    while not stopT:
        if not ws.connected:
            while not stopT:
                try:
                    ws.connect()
                    retry = 0
                    break
                except:
                    retry = retry + 1
                    print('\rSocket connection failed. Try : %d' % retry, end='')
                    del ws
                    ws = TermClient(server_config['ip'], {'username': 'term1', 'cid': cid})
                sleep(1)
            sleep(5)
        else:
            while not outQ.empty():
                ws.send(TermClient.make_msg('bs', {'bs': outQ.get()}))
    try:
        ws.close()
    except:
        print('Couldn\'t close socket')


if __name__ == '__main__':
    main()
