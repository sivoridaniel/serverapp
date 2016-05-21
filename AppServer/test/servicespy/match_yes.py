import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de match yes http://localhost:3000/match/yes')
        parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-ua','--userAccepted',type=str,help='Nombre de usuario aceptado', required = True)

        args = parser.parse_args()
        user = args.user
        userAccepted = args.userAccepted

        return user,userAccepted 

user, userAccepted = get_args()

r = requests.get("http://localhost:3000/match/yes", data = {"idUser":user,"idUserAccepted":userAccepted})

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL MATCH YES"
    data = json.loads(r.text)
    result = data['result']
    assert( result == 'success'),"MATCHEO NO VALIDO: %s"%result
    print result
except AssertionError, e:
    print 'NOK: %s'%e

