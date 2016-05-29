import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de match yes http://localhost:3000/match/yes')
        parser.add_argument('-u1','--user1',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-u2','--user2',type=str,help='Nombre de usuario aceptado', required = True)

        args = parser.parse_args()
        user = args.user1
        userAccepted = args.user2

        return user1,user2 

user1, user2 = get_args()

data = '{\"idFrom\":'+user1+', \"idTo\":'+user2+'}'

r = requests.post("http://localhost:3000/match/yes", data = data, headers = {"content-type": "application/json"})

try: 
    assert( r.status_code == 200 ),"ERROR LLAMANDO A MATCH YES"
    print r.json()
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data


