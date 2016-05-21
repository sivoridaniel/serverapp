import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de match no http://localhost:3000/match/no')
        parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-ur','--userRejected',type=str,help='Nombre de usuario rechazado', required = True)

        args = parser.parse_args()
        user = args.user
        userRejected = args.userRejected

        return user,userRejected

user, userRejected = get_args()

r = requests.get("http://localhost:3000/match/no", data = {"idUser":user,"idUserRejected":userRejected})

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL MATCH NO"
    data = json.loads(r.text)
    result = data['result']
    assert( result == 'success'),"ERROR MATCHEO REJECT: %s"%result
    print result
except AssertionError, e:
    print 'NOK: %s'%e

