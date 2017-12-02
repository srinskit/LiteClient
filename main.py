from time import sleep
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
        path += '?username=' + self.cred['username'] + '&type=terminal' + '&cid=' + self.cred['cid'] + '&password=' + \
                self.cred['password']
        WebSocketClient.__init__(self, path)

    @staticmethod
    def make_msg(typ, data):
        msg = {'type': typ, 'data': data}
        return dumps(msg)

    def process_msg(self, msg):
        typ = msg['type']
        if typ == 'auth':
            self.after_auth()
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
        json_msg = loads(m.data.decode("utf-8"))
        print_debug('From srv : ' + str(json_msg))
        self.process_msg(json_msg)

    def send(self, m, **kwargs):
        print_debug('To srv : %s' % str(m))
        try:
            super().send(m)
        except:
            pass


def fake_it(bs):
    typ = bs[:5]
    if typ == '00010':
        bri = '00'
        # input('Bri of ' + str(int(bs[5:15], 2)) + ' ')
        toServer.put(bs[:15] + bri + bs[18:])


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


def serial_manager():
    global time_to_read, serialDev
    while ProgramController.run:
        try:
            serialDev = Serial('/dev/ttyACM0', 38400, timeout=10)
            sleep(5)
            while ProgramController.run:
                try:
                    if time_to_read:
                        ack = serialDev.read(29)
                        if ack == b'':
                            continue
                        print_debug('Got from ard : ' + ack.decode("utf-8"))
                        time_to_read = False
                    else:
                        if not fromServer.empty():
                            bs = fromServer.get()
                            bs = '0' + bs
                            serialDev.write(bs.encode('ascii'))
                            fake_it(bs)
                            # time_to_read = True
                            sleep(2)
                except:
                    print_warn('Failed to send/receive to/from serial dev')
                    sleep(2)
                    break
        except:
            print_warn('Failed to connect to serial dev')
            sleep(1)
        finally:
            if serialDev is not None:
                serialDev.close()


time_to_read = True
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
