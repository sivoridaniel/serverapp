#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

class User:

	 def __init__(self, name, password, email):
            	self.name = name
               	self.password = password
        	self.email = email

  	 def reprJSON(self):
        	return dict(name=self.name,password = self.password,email = self.email) 

def get_args():

	parser = argparse.ArgumentParser('Llamada a creación de nuevo usuario http://localhost:3000/user/newuser')
      	parser.add_argument('-n','--name',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-p','--password',type=str,help='Password', required = True)
	parser.add_argument('-e','--email',type=str,help='Correo electronico', required = True)
	args = parser.parse_args()
	name = args.name
	password = args.password
	email = args.email
	
	return name, password, email

name, password, email = get_args()

data = User(name,password,email)

data = '{\"user\":'+json.dumps(data.reprJSON())+'}'

headers = {"content-type": "application/json"}

r = requests.post("http://localhost:3000/user/newuser", data = data, headers = headers, stream=True)

try:
   assert( r.status_code == 200 ),"ERROR LLAMADA CREANDO NUEVO USUARIO"
   # for raw_rsvp in r.iter_lines():
    #	if raw_rsvp:
     #   	rsvp = json.loads(raw_rsvp)
   #  for chunk in r.iter_content(0):
	#	print chunk
   data = json.loads(r.text)
   print data

except AssertionError, e:
    print 'NOK: %s'%e
