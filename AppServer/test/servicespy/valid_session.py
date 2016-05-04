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

try:
	r = requests.get("http://localhost:3000/valid_session", data = {"token":token})
        
	data = json.loads(r.text) 
        
	result = data['result']
        
	assert(result == 'status ok'),"SESION NO VALIDA. Return %s"%result
	
	print result

except AssertionError, e:
    	print 'NOK: %s'%e
