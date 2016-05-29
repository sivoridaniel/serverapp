import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de match confirm http://localhost:3000/match/confirm')
        parser.add_argument('-u1','--user1',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-u2','--user2',type=str,help='Nombre de usuario a confirmar', required = True)

        args = parser.parse_args()
        user1 = args.user1
        user2 = args.user2

        return user1,user2

user1, user2 = get_args()

data = '{\"idFrom\":'+user1+', \"idTo\":'+user2+'}'

r = requests.post("http://localhost:3000/match/confirm", data = data, headers = {"content-type": "application/json"})

try: 
    assert( r.status_code == 200 ),"ERROR LLAMANDO A MATCH CONFIRM"
    print r.json()
except AssertionError, e:
    print 'NOK: %s'%e
    print r.status_code
    data = json.loads(r.text)
    print data


