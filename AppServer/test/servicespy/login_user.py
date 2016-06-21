#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class User:
         def __init__(self, email, password):
                self.email = email
                self.password = password

         def reprJSON(self):
                return dict(email=self.email,password = self.password)

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio login http://localhost:3000/login_user')
        parser.add_argument('-e','--email',type=str,help='Email del usuario', required = True)
        parser.add_argument('-p','--password',type=str,help='Password', required = True)
        
        args = parser.parse_args()
        email = args.email
        password = args.password
        
	return email,password

email, password = get_args()

user = User(email, password)

data = '{\"user\":'+json.dumps(user.reprJSON())+'}'

headers = {"content-type": "application/json"}


r = requests.get("http://200.16.116.146:3000/login_user", data = data, headers = headers)

try:
    token = r.headers.get('Token')
    data = r.text
    print data
    print token
except AssertionError, e:
    print 'NOK: %s'%e

