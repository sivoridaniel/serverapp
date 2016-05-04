#!/usr/bin/python
# coding=utf-8

import argparse
import requests
import json
import sys

def get_args():

	parser = argparse.ArgumentParser('Llamada a creación de nuevo usuario http://localhost:3000/user/newuser')
      	parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
	parser.add_argument('-p','--password',type=str,help='Password', required = True)
	parser.add_argument('-n','--fname',type=str,help='Nombre', required = True)
	parser.add_argument('-a','--lname',type=str,help='Apellido', required = True)
	parser.add_argument('-e','--mail',type=str,help='Correo electronico', required = True)
	
	args = parser.parse_args()
	user = args.user
	password = args.password
	fname = args.fname
	lname = args.lname
	mail = args.mail
	
	return user, password, fname, lname, mail

user, password, fname, lname, mail = get_args()

r = requests.get("http://localhost:3000/user/newuser", data = {"name":user,"password":password,"firstName":fname,"lastName":lname,"email":mail})

try:
    assert( r.status_code == 200 ),"ERROR LLAMADA CREANDO NUEVO USUARIO"
    data = json.loads(r.text)
    result = data['result']
    assert( result == 'status ok'),"ERROR CREANDO EL USUARIO %s"%result    
    print result

except AssertionError, e:
    print 'NOK: %s'%e
