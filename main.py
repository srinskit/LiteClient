import traceback
from time import sleep
import time as time
from serial import Serial
from ws4py.client.threadedclient import WebSocketClient
from json import dumps, loads
from multiprocessing import Queue
import threading
import signal
import logging

fromServer = Queue()
toServer = Queue()
serverConfig = {}
socketThread = None
serialThread = None
socketDev = None
serialDev = None
cid = None
print_mode = True


def print_debug(msg):
    if print_mode:
        print(msg)
    else:
        print_debug(msg)


def print_warn(msg):
    if print_mode:
        print(msg)
    else:
        print_warn(msg)


def print_error(msg):
    if print_mode:
        print(msg)
    else:
        print_error(msg)


class ProgramController:
    run = True

    def __init__(self):
        signal.signal(signal.SIGINT, ProgramController.terminate)
        signal.signal(signal.SIGTERM, ProgramController.terminate)

    @staticmethod
    def terminate(*_):
        print_debug('Got SIGTERM')
        ProgramController.run = False


class TermClient(WebSocketClient):
    cred = {}

    def __init__(self, path):
        self.connected = False
        self.cred = devConfig
        path += '?type=terminal&username=%s&password=%s' % (self.cred['username'], self.cred['password'])
        WebSocketClient.__init__(self, path)

    @staticmethod
    def make_msg(typ, data):
        msg = {'type': typ, 'data': data}
        return dumps(msg)

    def process_msg(self, msg):
        typ = msg['type']
        if typ == 'auth':
            if msg['data']['state'] == 'pass':
                self.after_auth()
            else:
                print_error('Socket Auth Failed.')
                ProgramController.run = False
        elif typ == 'bs':
            fromServer.put(msg['data']['bs'])

    def opened(self):
        print_debug('Connection Opened!')

    def after_auth(self):
        self.connected = True
        self.send(self.make_msg('addListener', {'loc': 'lamps', 'cid': self.cred['cid']}))

    def closed(self, code, reason=None):
        self.connected = False
        print_debug('Connection Closed!' + str(code) + str(reason))

    def received_message(self, m):
        try:
            json_msg = loads(m.data.decode("utf-8"))
            print_debug('From srv : %s' % str(json_msg))
            self.process_msg(json_msg)
        except:
            print('FOoooooo' + m.data.decode("utf-8"))

    def send(self, m, **kwargs):
        print_debug('To   srv : %s' % str(m))
        try:
            super().send(m)
        except:
            pass


def compress(bs):
    tmp = ''
    for i in range(5):
        tmp += str(chr(int(bs[i * 7:(i + 1) * 7], 2)))
    return tmp


def extract(tmp):
    bs = ''
    for i in range(4):
        bs += format(ord(tmp[i]), '07b')
    bs += format(ord(tmp[4]), '01b')
    return bs


def socket_manager():
    global socketDev, cid
    retry = 0
    socketDev = TermClient(serverConfig['ss_ip'])
    while ProgramController.run:
        if not socketDev.connected:
            while ProgramController.run:
                try:
                    socketDev.connect()
                    retry = 0
                    break
                except:
                    retry = retry + 1
                    print_warn('Socket connection failed. Try : %d' % retry)
                    del socketDev
                    sleep(2)
                    socketDev = TermClient(serverConfig['ss_ip'])
            socketDev.connected = True
        else:
            while ProgramController.run and not toServer.empty():
                socketDev.send(TermClient.make_msg('bs', {'bs': toServer.get()}))
    try:
        socketDev.close()
    except:
        print_error('Failed to close socket')


def simulate(bs):
    if bs[:5] == '00010':
        # sleep(5)
        ret = None
        if int(bs[5:15], 2) == 1:
            ret = '000'
        elif int(bs[5:15], 2) == 2:
            ret = '000'
        elif int(bs[5:15], 2) == 3:
            # sleep(5)
            ret = '011'
        # toServer.put(bs[:15] + input('status of lamp ' + str(int(bs[5:15], 2)) + ': ') + bs[18:])
        toServer.put(bs[:15] + ret + bs[18:])


def serial_manager():
    global time_to_read, serialDev
    while ProgramController.run:
        try:
            serialDev = Serial('/dev/ttyACM0', 38400, timeout=1)
            sleep(2)
            while ProgramController.run:
                try:
                    if serialDev.in_waiting:
                        ascii_ack = serialDev.read(5)
                        print_debug('Got from ard : ' + ' '.join(([str(ch) for ch in ascii_ack])))
                        ack = ascii_ack.decode("utf-8")
                        if len(ack) == 5:
                            toServer.put(extract(ack))
                            # time_to_read = False
                    if not fromServer.empty():
                        bs = fromServer.get()
                        # simulate(bs)
                        bs = compress(bs)
                        print_debug('Sent to ard : ' + ' '.join(([str(ord(ch)) for ch in bs])))
                        serialDev.write(bs.encode('ascii'))
                        sleep(.2)
                        # time_to_read = True
                except:
                    print(traceback.format_exc())
                    print_warn('Failed to send/receive to/from serial dev')
                    sleep(1)
                    break
        except:
            print_warn('Failed to connect to serial dev')
            sleep(1)
        finally:
            if serialDev is not None:
                serialDev.close()


# time_to_read = True
if __name__ == '__main__':
    pc = ProgramController()
    logging.basicConfig(filename='log.log', level=logging.DEBUG)
    print_debug('Start')
    serverConfig = loads(open('server_config.json').read())
    devConfig = loads(open('device_config.json').read())
    if serverConfig.get('hs_ip') is None:
        print_error('No server')
        quit(1)
    time_to_read = False
    cid = devConfig['cid']
    socketThread = threading.Thread(target=socket_manager)
    socketThread.start()
    serialThread = threading.Thread(target=serial_manager)
    serialThread.start()
    socketThread.join()
    serialThread.join()
    print_debug('End')
