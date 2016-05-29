#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio get new messages http://localhost:3000/chat/new')
        parser.add_argument('-u1','--id1',type=str,help='Id de usuario1', required = True)
        parser.add_argument('-u2','--id2',type=str,help='Id de usuario2', required = True)
        
        args = parser.parse_args()
        iduser1 = args.id1
        iduser2 = args.id2

        
	return iduser1,iduser2

iduser1, iduser2 = get_args()

headers = {"content-type": "application/json"}

params = {
'idFrom':iduser1,
'idTo':iduser2
}

r = requests.get("http://localhost:3000/chat/new", params = params , headers = headers)

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL NEW MESSAGES"
    print r.json()   
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data

