#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio get new matches http://localhost:3000/match')
        parser.add_argument('-u','--id',type=str,help='Id de usuario', required = True)
        parser.add_argument('-t','--token',type=str,help='Token', required = True)
        
        args = parser.parse_args()
        iduser = args.id
        token = args.token

        
	return iduser, token

iduser, token = get_args()

headers = {"content-type": "application/json", "token":token}

params = {
'id':iduser
}

r = requests.get("http://localhost:3000/match", params = params , headers = headers)

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL NEW MATCHES"
    print r.json()
    header = r.headers.get('Token')	
    print 'Header: %s'%header    
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data

