#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio login http://localhost:3000/candidates')
        parser.add_argument('-i','--id',type=str,help='Id de usuario', required = True)
        
        args = parser.parse_args()
        iduser = args.id

        
	return iduser

iduser = get_args()

headers = {"content-type": "application/json"}

params = {
'id':iduser
}

r = requests.get("http://localhost:3000/candidates", params = params , headers = headers)

try:   
    data = r.text    
    print r.json()
except AssertionError, e:
    print 'NOK: %s'%e
