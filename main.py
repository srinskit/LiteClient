from time import sleep
from serial import Serial
from ws4py.client.threadedclient import WebSocketClient
from json import dumps, loads
from multiprocessing import Queue
import threading
import sys
import signal
import logging

fromServer = Queue()
toServer = Queue()
serverConfig = {}
socketThread = None
serialThread = None
socketDev = None
serialDev = None
CID = None


class ProgramController:
    run = True

    def __init__(self):
        signal.signal(signal.SIGINT, ProgramController.terminate)
        signal.signal(signal.SIGTERM, ProgramController.terminate)

    @staticmethod
    def terminate(*_):
        logging.debug('Got sigterm')
        ProgramController.run = False


class TermClient(WebSocketClient):
    cred = {}

    def __init__(self, path, cred):
        self.connected = False
        self.cred = cred
        path += '?username=' + cred['username'] + '&type=terminal' + '&cid=' + cred['CID']
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
        logging.debug('Connection Opened!')

    def after_auth(self):
        self.connected = True
        self.send(self.make_msg('addListener', {'loc': 'lamps', 'CID': self.cred['CID']}))

    def closed(self, code, reason=None):
        self.connected = False
        logging.debug('Connection Closed!', code, reason)

    def received_message(self, m):
        json_msg = loads(m.data.decode("utf-8"))
        logging.debug('From srv : ' + str(json_msg))
        self.process_msg(json_msg)

    def send(self, m, **kwargs):
        logging.debug('To srv : %s' % str(m))
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
    global socketDev, CID
    retry = 0
    socketDev = TermClient(serverConfig['ip'], {'username': 'term1', 'CID': CID})
    while ProgramController.run:
        if not socketDev.connected:
            while ProgramController.run:
                try:
                    socketDev.connect()
                    retry = 0
                    break
                except:
                    retry = retry + 1
                    logging.warning('Socket connection failed. Try : %d' % retry)
                    del socketDev
                    socketDev = TermClient(serverConfig['ip'], {'username': 'term1', 'CID': CID})
                sleep(1)
            sleep(1)
        else:
            while ProgramController.run and not toServer.empty():
                socketDev.send(TermClient.make_msg('bs', {'bs': toServer.get()}))
    try:
        socketDev.close()
    except:
        logging.error('Failed to close socket')


def serial_manager():
    global time_to_read, serialDev
    logging.debug('Reached')
    while ProgramController.run:
        try:
            serialDev = Serial('/dev/ttyUSB0', 9600, timeout=10)
            while ProgramController.run:
                try:
                    if time_to_read:
                        ack = serialDev.read(29)
                        if ack == b'':
                            continue
                        logging.debug('Got from ard : ' + ack.decode("utf-8"))
                        time_to_read = False
                    else:
                        if not fromServer.empty():
                            bs = fromServer.get()
                            serialDev.write(bs.encode('ascii'))
                            fake_it(bs)
                            time_to_read = True
                except:
                    logging.warning('Failed to send/receive to/from serial dev')
                    sleep(1)
                    break
        except:
            logging.warning('Failed to connect to serial dev')
            sleep(1)
        finally:
            if serialDev is not None:
                serialDev.close()


time_to_read = True
if __name__ == '__main__':
    pc = ProgramController()
    logging.basicConfig(filename='log.log', level=logging.DEBUG)
    logging.debug('Start')
    s_config = open('server.config')
    for line in s_config:
        pair = line.split(':', 1)
        serverConfig[pair[0]] = pair[1]
    if serverConfig.get('ip') is None:
        logging.error('No server')
        quit(1)
    time_to_read = True
    try:
        # CID = '1'
        CID = str(int(sys.argv[1]))
    except (IndexError, ValueError) as e:
        logging.error('Invalid CID; Exception : %s' % str(e))
        quit(1)
    socketThread = threading.Thread(target=socket_manager)
    socketThread.start()
    serialThread = threading.Thread(target=serial_manager)
    serialThread.start()
    socketThread.join()
    serialThread.join()
    logging.debug('End')
