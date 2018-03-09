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
            fromServer.put(msg['bs'])
        else:
            fromServer.put(msg['bs'])

    def opened(self):
        print_debug('Connection Opened!')

    def after_auth(self):
        self.connected = True

    def closed(self, code, reason=None):
        self.connected = False
        print_debug('Connection Closed!' + str(code) + str(reason))

    def received_message(self, m):
        try:
            json_msg = loads(m.data.decode("utf-8"))
            print_debug('From srv : %s' % str(json_msg))
            self.process_msg(json_msg)
        except:
            print('Couldn\'t parse: ' + m.data.decode("utf-8"))

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
                    if retry == 1:
                        fromServer.put(compress('11111' + '0' * 24))
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
        # sleep(1 * int(bs[5:15], 2))
        ret = None
        if int(bs[5:15], 2) == 1:
            # sleep(10)
            ret = '000'
        elif int(bs[5:15], 2) == 2:
            # sleep(10)
            ret = '000'
        elif int(bs[5:15], 2) == 3:
            # sleep(5)
            ret = '011'
        # toServer.put(bs[:15] + input('status of lamp ' + str(int(bs[5:15], 2)) + ': ') + bs[18:])
        toServer.put(bs[:15] + ret + bs[18:])


def match(req, res):
    if req is None or res is None:
        return True
    # Make 5th bit of req zero
    req = req[:4] + '0' + req[5:]
    req, res = extract(req), extract(res)
    # Check id match
    if req[2:3] != res[2:3]:
        return False
    # If status
    req_op, res_op = req[:5], res[:5]
    if req_op == '00010':
        return res_op == '00010'
    # Else has to be exact
    return req == res


def serial_manager():
    global serialDev
    print_failure, print_success = True, True
    resend_count, max_resend = 0, 2
    response_time, timeout, timeout_q = 0, 6, []
    sent_instruction, resend_instruction = None, None
    while ProgramController.run:
        try:
            if term1:
                serialDev = Serial('/dev/ttyACM0', 38400, timeout=2)
            else:
                serialDev = Serial('/dev/ttyACM1', 38400, timeout=2)
            if print_success:
                print('Connected to serial dev')
                print_success = False
            print_failure = True
            sleep(3)
            while ProgramController.run:
                try:
                    if serialDev.in_waiting >= 5:
                        # If at least 5 bytes are in buffer
                        raw_response = serialDev.read(5)
                        print_debug('Got frm ard : ' + ' '.join(([str(ch) for ch in raw_response])))
                        response = raw_response.decode('utf-8')
                        op_code = response[0]
                        if op_code == 28 or op_code == 60:
                            # Lone Msg
                            toServer.put(response)
                        elif match(sent_instruction, response):
                            # It's match!
                            sent_instruction = None
                            resend_instruction = None
                            send_count = 0
                        else:
                            # Mismatch or timed-out req's res
                            for req in timeout_q:
                                if match(req, response):
                                    # Timed-out req's res
                                    resend_instruction = None
                                    break
                            else:
                                # Mismatch
                                resend_instruction = sent_instruction
                            sent_instruction = None
                    if sent_instruction is None or not fromServer.empty():
                        if resend_instruction is None:
                            sent_instruction = ''.join([chr(int(num)) for num in fromServer.get().split()])
                            serialDev.write(sent_instruction.encode('ascii'))
                            print_debug('Sent to ard : ' + ' '.join(([str(ord(ch)) for ch in sent_instruction])))
                            resend_count = 0
                        elif resend_count < max_resend:
                            sent_instruction = resend_instruction
                            serialDev.write(sent_instruction.encode('ascii'))
                            print_debug('Rest to ard : ' + ' '.join(([str(ord(ch)) for ch in sent_instruction])))
                            resend_count += 1
                        else:
                            print_debug('Failed      :' + ' '.join(([str(ord(ch)) for ch in resend_instruction])))
                            if response_time >= timeout and (resend_instruction[0] == 8 or resend_instruction[0] == 12):
                                # Notify status timed-out
                                toServer.put(compress("10011" + resend_instruction[5:]))
                            resend_instruction = None
                            sent_instruction = None
                        response_time = 0
                    else:
                        response_time += .2
                        sleep(.2)
                        if response_time >= timeout:
                            timeout_q.append(sent_instruction)
                            resend_instruction = sent_instruction
                            sent_instruction = None
                except:
                    print(traceback.format_exc())
                    print_warn('Failed to send/receive to/from serial dev')
                    sleep(1)
                    break
        except:
            if print_failure:
                print_warn('Not connected to serial dev')
                print_failure = False
            print_success = True
            sleep(1)
        finally:
            if serialDev is not None:
                serialDev.close()


term1 = False
if __name__ == '__main__':
    pc = ProgramController()
    logging.basicConfig(filename='log.log', level=logging.DEBUG)
    print_debug('Start')
    serverConfig = loads(open('server_config.json').read())
    if term1:
        devConfig = loads(open('device_config1.json').read())
    else:
        devConfig = loads(open('device_config2.json').read())
    if serverConfig.get('hs_ip') is None:
        print_error('No server')
        quit(1)
    cid = devConfig['cid']
    socketThread = threading.Thread(target=socket_manager)
    socketThread.start()
    serialThread = threading.Thread(target=serial_manager)
    serialThread.start()
    socketThread.join()
    serialThread.join()
    print_debug('End')
