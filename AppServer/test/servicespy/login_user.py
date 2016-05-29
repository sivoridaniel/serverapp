#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class User:
         def __init__(self, id, password):
                self.id = id
                self.password = password

         def reprJSON(self):
                return dict(id=self.id,password = self.password)

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio login http://localhost:3000/login_user')
        parser.add_argument('-id','--id',type=str,help='Id de usuario', required = True)
        parser.add_argument('-p','--password',type=str,help='Password', required = True)
        
        args = parser.parse_args()
        id_ = args.id
        password = args.password
        
	return id_,password

id_, password = get_args()

user = User(id_, password)

data = '{\"user\":'+json.dumps(user.reprJSON())+'}'

headers = {"content-type": "application/json"}


r = requests.get("http://localhost:3000/login_user", data = data, headers = headers)

try:
    token = r.headers.get('Token')
    data = r.text
    print 'Token: %s'%token
    print 'Return: %s'%data
except AssertionError, e:
    print 'NOK: %s'%e

