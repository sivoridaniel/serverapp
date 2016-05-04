import argparse
import requests
import json
import sys

def get_args():

        parser = argparse.ArgumentParser('Llamada al servicio de confirm match http://localhost:3000/match/confirm')
        parser.add_argument('-u','--user',type=str,help='Nombre de usuario', required = True)
        parser.add_argument('-uc','--userConfirmed',type=str,help='Nombre de usuario confirmado', required = True)

        args = parser.parse_args()
        user = args.user
        userConfirmed = args.userConfirmed

        return user,userConfirmed

user, userConfirmed = get_args()

r = requests.get("http://localhost:3000/match/confirm", data = {"idUser":user,"idUserConfirmed":userConfirmed})

try:
    assert( r.status_code == 200 ),"ERROR LLAMANDO AL MATCH CONFIRMED"
    data = json.loads(r.text)
    result = data['result']
    assert( result == 'success'),"ERROR MATCHEO CONFIRMACION: %s"%result
    print result
except AssertionError, e:
    print 'NOK: %s'%e

