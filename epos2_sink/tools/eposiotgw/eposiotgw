#!/usr/bin/env python3

# To get an unencrypted PEM (without passphrase):
# openssl rsa -in certificate.pem -out certificate_unencrypted.pem

import os
import sys
import time
import serial
import argparse
import requests

parser = argparse.ArgumentParser(description='EPOS Serial->IoT Gateway')

required = parser.add_argument_group('required named arguments')
required.add_argument('-c','--certificate', help='Your PEM certificate', required=True)

parser.add_argument('-d','--dev', help='EPOSMote III device descriptor file', default='/dev/ttyACM0')
parser.add_argument('-t','--timeout', help='Timeout for reading from mote', default=600)
parser.add_argument('-u','--url', help='Post URL', default='https://iot.lisha.ufsc.br/api/put.php')
parser.add_argument('-a','--attach_url', help='Attach URL', default='https://iot.lisha.ufsc.br/api/attach.php')
parser.add_argument('-j','--json', help='Use JSON API', action='store_true')
parser.add_argument('-D','--domain', help='Data domain', default='')
parser.add_argument('-U','--username', help='Data domain username', default='')
parser.add_argument('-P','--password', help='User password', default='')

args = vars(parser.parse_args())
DEV = args['dev']
TIMEOUT = int(args['timeout'])
URL = args['url']
ATTACH_URL = args['attach_url']
MY_CERTIFICATE = args['certificate']
MY_CERTIFICATE = [args['certificate']+'.pem', args['certificate']+'.key']
DOMAIN = args['domain']
USERNAME = args['username']
PASSWORD = args['password']
JSON = args['json']

if JSON:
    import json
    import struct
    from struct import unpack

def init_mote():
    global DEV
    global TIMEOUT

    ok = False
    while not ok:
        try:
            print("Waiting for", DEV, "to appear")
            while not os.path.exists(DEV) or not os.access(DEV, os.W_OK):
                pass
            mote = serial.Serial(DEV, 115200, timeout = TIMEOUT, write_timeout = 10)
            ok = True
        except KeyboardInterrupt:
            raise
        except Exception as e:
            print("Exception caught:", e, file=sys.stderr)
            ok = False
            time.sleep(3)

    print("Mote open", file=sys.stderr)
    ts = bytes(str(int(time.time() * 1000000)), 'ascii')
    try:
        mote.write(ts + b'X')
        print("epoch written", file=sys.stderr)
    except KeyboardInterrupt:
        raise
    except serial.serialutil.SerialTimeoutException:
        pass

    print("init_mote() done", file=sys.stderr)
    return mote

def to_json(data):
    value = unpack("<d", data[0:8])[0]
    error = unpack("<B", data[8:9])[0]
    confidence = unpack("<B", data[9:10])[0]
    x = unpack("<i", data[10:14])[0]
    y = unpack("<i", data[14:18])[0]
    z = unpack("<i", data[18:22])[0]
    t = unpack("<Q", data[22:30])[0]

    #version = unpack("<B", data[46:47])[0]
    version = "1.1" # Version 1, Static
    unit = unpack("<I", data[47:51])[0]
    center_x = unpack("<i", data[51:55])[0]
    center_y = unpack("<i", data[55:59])[0]
    center_z = unpack("<i", data[59:63])[0]
    r = unpack("<I", data[63:67])[0]
    t0 = unpack("<Q", data[67:75])[0]
    t1 = unpack("<Q", data[75:83])[0]

    if DOMAIN or USERNAME or PASSWORD:
        credentials = {}
        if DOMAIN:
            credentials['domain'] = DOMAIN
        if USERNAME:
            credentials['username'] = USERNAME
        if PASSWORD:
            credentials['password'] = PASSWORD

    data_ret_obj = {
            'smartdata' :
            [
                {
                    'version' : version,
                    'unit' : unit,
                    'error' : error,
                    'confidence' : confidence,
                    'x' : x,
                    'y' : y,
                    'z' : z,
                    'value' : value,
                    'time' : t,
                    'mac' : 0
                }
            ]
        }

    if DOMAIN or USERNAME or PASSWORD:
        data_ret_obj['credentials'] = credentials

    series_ret_obj = {
            'series' :
            {
                'version' : version,
                'unit' : unit,
                'x' : center_x,
                'y' : center_y,
                'z' : center_z,
                'r' : r,
                't0' : t0,
                't1' : t1
            }
        }

    if DOMAIN or USERNAME or PASSWORD:
        series_ret_obj['credentials'] = credentials

    data_ret = json.dumps(data_ret_obj)
    series_ret = json.dumps(series_ret_obj)

    return series_ret, data_ret

attached = []

if __name__ == "__main__":
    mote = init_mote()

    session = requests.Session()
    if JSON:
        session.headers = {'Content-type' : 'application/json'}
    else:
        session.headers = {'Connection': 'close', 'Content-type' : 'application/octet-stream', 'Content-length' : 83}

    session.cert = MY_CERTIFICATE

    while True:
        try:
            data = mote.read(83)
        except KeyboardInterrupt:
            raise
        except Exception as e:
            print("Exception caught:", e, file=sys.stderr)
            data = b''

        if not len(data):
            mote.close()
            mote = init_mote()
        else:
            if JSON:
                [series, smartdata] = to_json(data)
                if not series in attached:
                    response = session.post(ATTACH_URL, series)
                    print("[", str(response.status_code), "] (", len(series), ") ", series, sep='')
                    attached.append(series)

                response = session.post(URL, smartdata)
                print("[", str(response.status_code), "] (", len(smartdata), ") ", smartdata, sep='')

            else:
                response = session.post(URL, data)

                print("[", str(response.status_code), "] (", len(data), ") ", data, sep='');
