#!/usr/bin/python
# coding=utf-8

import json
import argparse
import requests
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al validador de sesion de usuario http://localhost:3000/valid_session')
        parser.add_argument('-t','--token',type=str,help='Token', required = True)
        
        args = parser.parse_args()
        token = args.token
        
        return token

token = get_args()
headers = {"Content-Type":"application/json","token":token}
data=''

r = requests.get("http://localhost:3000/valid_session", data=data, headers=headers)
	
try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO A VALID SESSION"
    print r.json()   
    header = r.headers.get('Token')	
    print 'Header: %s'%header
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data

