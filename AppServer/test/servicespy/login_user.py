#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio login http://localhost:3000/login_user')
        parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-p','--password',type=str,help='Password', required = True)
        
        args = parser.parse_args()
        user = args.user
        password = args.password
        
	return user, password

user, password = get_args()



r = requests.get("http://localhost:3000/login_user", data = {"name":user,"password":password})

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL LOGIN"
    data = json.loads(r.text)
    result = data['result']
    assert( result == '200'),"AUTENTICACION NO VALIDA: %s"%result
    print result
except AssertionError, e:
    print 'NOK: %s'%e

