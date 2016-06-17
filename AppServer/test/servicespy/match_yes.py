import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de match yes http://localhost:3000/match/yes')
        parser.add_argument('-u1','--user1',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-u2','--user2',type=str,help='Nombre de usuario aceptado', required = True)
        parser.add_argument('-t','--token',type=str,help='Token', required = True)

        args = parser.parse_args()
        user1 = args.user1
        user2 = args.user2
	token = args.token

        return user1,user2,token 

user1, user2, token = get_args()

data = '{\"idFrom\":'+user1+', \"idTo\":'+user2+'}'

headers = {"content-type": "application/json", "token":token}

r = requests.post("http://localhost:3000/match/yes", data = data, headers = headers)

try: 
    assert( r.status_code == 200 ),"ERROR LLAMANDO A MATCH YES"
    print r.json()
    header = r.headers.get('Token')	
    print 'Header: %s'%header 
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data


