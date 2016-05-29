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
headers = {"Content-Type":"application/json","Accept":"text/plain","token":token}
data=''

try:
	r = requests.get("http://localhost:3000/valid_session", data=data, headers=headers)
	
	header = r.headers.get('Token')
	
	print 'Header: %s'%header
    
	data = json.loads(r.text) 
        
	status = data['status']
        
	assert(status == '200'),"SESION NO VALIDA. Return: %s"%status
	
	print 'Resultado: %s'%result
	print 'Status: %s'%status

except AssertionError, e:
    	print 'NOK: %s'%e
