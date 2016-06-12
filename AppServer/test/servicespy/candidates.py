#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio login http://localhost:3000/candidates')
        parser.add_argument('-u','--id',type=str,help='Id de usuario', required = True)
        
        args = parser.parse_args()
        iduser = args.id

        
	return iduser

iduser = get_args()

headers = {"content-type": "application/json"}

params = {
'id':iduser
}

r = requests.get("http://200.16.116.146:3000/candidates", params = params , headers = headers)

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO A CANDIDATES"
    print r.text
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = r.text
    print data

